package main

import (
	"github.com/game_engine/timer"
	"github.com/golang/protobuf/proto"
	"server/gameServer/game"
	"server/share/protocol"
)

import (
	"fmt"
	"net"
)

var conn2a net.Conn
var server_count int32

func init() {
	server_count = 0
}

func send2AccountMenber() {
	server_count += 1
	result2A := &protocol.Account_GameResult{
		Count:       proto.Int32(server_count),
		GameAddress: proto.String(game.ServerNoteAddress),
	}
	result2A.Pid = &protocol.Default_Account_GameResult_Pid
	fmt.Println(result2A)
	encObj, _ := proto.Marshal(result2A)
	conn2a.Write(encObj)
	game.Log.Info("send 2 Account message")
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func Handler4C(conn net.Conn) {
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

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.Game_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case protocol.GameMsgID_Game_Msg_Login:
			//登陆
			login := new(protocol.Account_LoginInfo)
			if err := proto.Unmarshal(buf[0:n], login); err == nil {
			}
		default:
		}
	}
}

func Deal4Client(listener net.Listener) {
	conn, err := listener.Accept()
	if CheckError(err) {
		go Handler4C(conn)
	}
}

func main() {
	//连接账号服务器
	var err2 error
	conn2a, err2 = net.Dial("tcp", game.Server2AccountAddress)
	if CheckError(err2) {
		//定时告诉账号服务器当前人数
		send2AccountMenber()
		timer.CreateTimer(game.DistanceTime, true, send2AccountMenber)
	}

	//监听玩家连接
	listener, err := net.Listen("tcp", game.ServerAddress)
	if CheckError(err) {
		for {
			Deal4Client(listener)
		}
	}

}
