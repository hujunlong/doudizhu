package main

import (
	"github.com/game_engine/timer"
	"github.com/golang/protobuf/proto"
	"server/gameServer/game"
	"server/share/global"
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
	//conn2a.Write()
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
		case global.LoginInfoId:
			//登陆

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
