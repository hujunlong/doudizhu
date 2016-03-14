package main

import (
	"github.com/game_engine/timer"
	"server/gameServer/game"
)

import (
	"fmt"
	"net"
)

var conn2a net.Conn
var server_count int32 = 0
var end = make(chan int)

func init() {
	server_count = 0
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func main() {
	//连接账号服务器
	var err error
	conn2a, err = net.Dial("tcp", game.Server2AccountAddress)
	if CheckError(err) {
		//定时告诉账号服务器当前人数
		send2AccountMenber()
		timer.CreateTimer(game.DistanceTime, true, send2AccountMenber)
		go Handler2A(conn2a)
		fmt.Println("come here Handler2A")
	}

	//监听玩家连接
	fmt.Println("come here Deal4Client")
	listener1, err1 := net.Listen("tcp", game.ServerAddress)
	if CheckError(err1) {
		go Deal4Client(listener1)
	}

	<-end
}
