package main

import (
	"github.com/game_engine/cache/redis"
	"github.com/golang/protobuf/proto"
	"net"
	"server/gameServer/game"
	"server/share/protocol"
	"strconv"
	"sync"
)

type Deal2A struct {
	server_note_address string//通知玩家需要登录的游戏服
	server_count int //在线人数
	server_id int //游戏服id
	deal2amx *sync.RWMutex
}

func (this *Deal2A) Init(server_note_address string,server_id int) {
	this.deal2amx = new(sync.RWMutex)
	this.server_note_address = server_note_address
	this.server_count = 0
	this.server_id = server_id
}

func (this *Deal2A) send2AccountMenber() {
	this.server_count += 1
	pid := protocol.AccountMsgID_Msg_GameResult
	result2A := &protocol.Account_GameResult{
		Pid:         &(pid),
		Count:       proto.Int32(int32(this.server_count)),
		GameAddress: proto.String(this.server_note_address),
	}

	encObj, _ := proto.Marshal(result2A)
	conn2a.Write(encObj)
}

func (this *Deal2A) Task(pid protocol.AccountMsgID, buf []byte, n int) {
	switch pid {
	case protocol.AccountMsgID_Msg_NoteGame: //账号服务器登陆成功后往 game服务器写入 playerid sn
		get_note := new(protocol.Account_NoteGame)
		if err := proto.Unmarshal(buf[0:n], get_note); err == nil {
			player_id := get_note.GetPlayerId()
			game.Log.Info("player_id = %d", player_id)
			this.NoteGame(int(player_id))
		}

	default:
	}
}

//防外挂 逻辑更严密
func (this *Deal2A)NoteGame(player_id int){
	this.deal2amx.Lock()
	defer this.deal2amx.Unlock()
	
	//role ID 基础值 100000 + game_id + player_id
	id :=  1000000 + this.server_id + player_id 
	
	key := "role:" + strconv.Itoa(id)
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

		typeStruct := new(protocol.Account_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		go this.Task(*typeStruct.Pid, buf, n)
	}
}
