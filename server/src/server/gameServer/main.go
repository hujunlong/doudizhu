package main

import (
	"github.com/game_engine/timer"
	"server/gameServer/game"
)

import (
	"fmt"
	"net"
)

var conn2a net.Conn //连接账号服务器
var end = make(chan int)
var config *game.Config
var gameMsg *game.GameMsg
var deal_2a *Deal2A
var deal_4c *Deal4C

func init() {
	config = new(game.Config)
	config.Init()

	gameMsg = new(game.GameMsg)
	gameMsg.Init(config)

	deal_2a = new(Deal2A)
	deal_2a.Init(config, gameMsg)

	deal_4c = new(Deal4C)
	deal_4c.Init()
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
	conn2a, err = net.Dial("tcp", config.Server2AccountAddress)
	if CheckError(err) {
		//定时告诉账号服务器当前人数
		deal_2a.send2AccountMenber()
		timer.CreateTimer(config.DistanceTime, true, deal_2a.send2AccountMenber)

		//接收account服务器发来的消息
		go deal_2a.Handler2A(conn2a)
	}

	//监听玩家连接
	listener1, err1 := net.Listen("tcp", config.ServerAddress)
	if CheckError(err1) {
		go deal_4c.Deal4Client(listener1)
	}

	<-end
}
