package main

import (
	//"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/gameServer/game"
	"server/share/protocol"
)

type Deal2A struct {
	config       *game.Config
	gameMsg      *game.GameMsg
	server_count int32
}

func (this *Deal2A) Init(config *game.Config, gameMsg *game.GameMsg) {
	this.config = config
	this.gameMsg = gameMsg
	this.server_count = 0
}

func (this *Deal2A) send2AccountMenber() {
	this.server_count += 1
	pid := protocol.AccountMsgID_Msg_GameResult
	result2A := &protocol.Account_GameResult{
		Pid:         &(pid),
		Count:       proto.Int32(this.server_count),
		GameAddress: proto.String(this.config.ServerNoteAddress),
	}

	encObj, _ := proto.Marshal(result2A)
	conn2a.Write(encObj)

}

func (this *Deal2A) thisTask(pid protocol.AccountMsgID, buf []byte, n int) {
	switch pid {
	case protocol.AccountMsgID_Msg_NoteGame: //账号服务器登陆成功后往 game服务器写入 playerid sn
		get_note := new(protocol.Account_NoteGame)
		if err := proto.Unmarshal(buf[0:n], get_note); err == nil {
			player_id := get_note.GetPlayerId()
			game.Log.Info("player_id = %d", player_id)
			this.gameMsg.NoteGame(player_id)
		}
	default:
	}
}

//注册完成 账号服务器通知game服务器
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

		go this.thisTask(*typeStruct.Pid, buf, n)
	}
}
