package main

import (
	"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/gameServer/game"
	"server/share/protocol"
)

func Handler4C(conn net.Conn) {
	defer conn.Close()
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)
	for {
		fmt.Println(buf)
		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN {
			game.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.Game_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case protocol.GameMsgID_Game_Msg_RegisterRole:
			//注册
			register := new(protocol.Game_RegisterRole)
			if err := proto.Unmarshal(buf[0:n], register); err == nil {
				//game.loginGameServer()
			}
		default:
		}
	}
}

func Deal4Client(listener net.Listener) {
	for {
		conn, err := listener.Accept()
		if CheckError(err) {
			go Handler4C(conn)
		}
	}

}
