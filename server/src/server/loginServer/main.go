package main

import (
	"server/loginServer/account"
	//"server/share/protocol"
)

import (
	"fmt"
	"net"
	"sync"
)

var gameConnects map[string]account.Connect4C //key: "127.0.0.1:2422"
var end = make(chan int)
var accountMutex *sync.RWMutex

func init() {
	gameConnects = make(map[string]account.Connect4C)
	accountMutex = new(sync.RWMutex)
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func main() {

	//Deal4Client
	listener, err := net.Listen("tcp", account.Listen4CAddress)

	//Deal4Game
	listener2, err2 := net.Listen("tcp", account.Listen4GameAddress)

	if !CheckError(err) || !CheckError(err2) {
		return
	}

	go Deal4Client(listener)
	go Deal4GameServer(listener2)
	<-end
}
