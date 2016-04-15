package main

import (
	"net"
	"server/gameServer/game"
	"server/share/protocol"
	"strconv"
	"sync"

	"fmt"

	"github.com/game_engine/cache/redis"
	"github.com/golang/protobuf/proto"
)

type Deal2A struct {
	server_note_address string //通知玩家需要登录的游戏服
	server_count        int32  //在线人数
	server_id           int32  //游戏服id
	deal2amx            *sync.RWMutex
}

func (this *Deal2A) Init() {
	this.deal2amx = new(sync.RWMutex)
	this.server_note_address = sys_config.ServerNoteAddress
	this.server_count = 0
	this.server_id = sys_config.GameId

	fmt.Println("this.server_note_address =", this.server_note_address, "this.server_id=", this.server_id)
}

func (this *Deal2A) send2AccountMenber() {
	this.server_count += 1
	result2A := &protocol.Account_GameResult{
		Count:       proto.Int32(this.server_count),
		GameAddress: proto.String(this.server_note_address),
	}

	encObj, _ := proto.Marshal(result2A)
	SendPackage(conn2a, 101, encObj)

}

func (this *Deal2A) Task(pid int32, buf []byte, n int) {
	switch pid {
	case 102: //账号服务器登陆成功后往 game服务器写入 playerid sn
		get_note := new(protocol.Account_NoteGame)
		if err := proto.Unmarshal(buf[0:n], get_note); err == nil {
			player_id := get_note.GetPlayerId()
			game.Log.Info("player_id = %d", player_id)
			this.NoteGame(player_id)
		}

	default:
	}
}

//防外挂 逻辑更严密
func (this *Deal2A) NoteGame(player_id int32) {
	this.deal2amx.Lock()
	defer this.deal2amx.Unlock()

	id := this.server_id + player_id

	key := "role:" + strconv.Itoa(int(id))
	is_exists, _ := redis.Exists(key)

	if !is_exists {
		redis.Add(key, "")
	}
}

//账号服务器通知game服务器 消息下发
func (this *Deal2A) Handler2A(conn net.Conn) {
	defer conn.Close()
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)
	for {
		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN {
			game.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包头
		_, head_pid := GetHead(buf)
		go this.Task(head_pid, buf[8:], n-8)
	}
}
