package main

import (
	"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/gameServer/game"
	"server/share/protocol"
)

func send2AccountMenber() {
	server_count += 1
	pid := protocol.AccountMsgID_Msg_GameResult
	result2A := &protocol.Account_GameResult{
		Pid:         &(pid),
		Count:       proto.Int32(server_count),
		GameAddress: proto.String(game.ServerNoteAddress),
	}

	encObj, _ := proto.Marshal(result2A)
	conn2a.Write(encObj)
	game.Log.Info("send 2 Account message")
}

//注册完成 账号服务器通知game服务器
func Handler2A(conn net.Conn) {
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

		fmt.Println("*typeStruct.Pid:", *typeStruct.Pid)
		switch *typeStruct.Pid {
		case protocol.AccountMsgID_Msg_NoteGame: //账号服务器登陆成功后往 game服务器写入 playerid sn
			fmt.Println("come here now")
			get_note := new(protocol.Account_NoteGame)
			if err := proto.Unmarshal(buf[0:n], get_note); err == nil {
				player_id := get_note.GetPlayerId()
				game.NoteGame(player_id)
			}
		default:
		}
	}
}
