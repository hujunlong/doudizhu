package main

import (
	"bytes"
	"encoding/binary"

	"github.com/game_engine/timer"
)

import (
	"fmt"
	"net"
)

var conn2a net.Conn //连接账号服务器
var end = make(chan int)

var deal_2a *Deal2A
var deal_4c *Deal4C

func init() {

	deal_4c = new(Deal4C)
	deal_4c.Init()

	deal_2a = new(Deal2A)
	deal_2a.Init()

}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func SendPackage(conn net.Conn, pid int32, body []byte) {

	len := 8 + len(body)
	var len_32 = int32(len)

	len_buf := bytes.NewBuffer([]byte{})
	binary.Write(len_buf, binary.BigEndian, len_32)

	pid_buf := bytes.NewBuffer([]byte{})
	binary.Write(pid_buf, binary.BigEndian, pid)

	msg := append(len_buf.Bytes(), pid_buf.Bytes()...)
	msg2 := append(msg, body...)
	conn.Write(msg2)
}

func GetHead(buf []byte) (int32, int32) {
	if len(buf) < 8 {
		return 0, 0
	}

	var head_len int32 = 0
	var head_pid int32 = 0
	buffer_len := bytes.NewBuffer(buf[0:4])
	buffer_pid := bytes.NewBuffer(buf[4:8])
	binary.Read(buffer_len, binary.BigEndian, &head_len)
	binary.Read(buffer_pid, binary.BigEndian, &head_pid)
	return head_len, head_pid
}

func main() {
	//连接账号服务器

	var err error
	conn2a, err = net.Dial("tcp", sys_config.Server2AccountAddress)
	if CheckError(err) {
		//定时告诉账号服务器当前人数
		deal_2a.send2AccountMenber()
		timer.CreateTimer(sys_config.DistanceTime, true, deal_2a.send2AccountMenber)

		//接收account服务器发来的消息
		go deal_2a.Handler2A(conn2a)
	}

	//监听玩家连接
	listener1, err1 := net.Listen("tcp", sys_config.ServerAddress)
	if CheckError(err1) {
		go deal_4c.Deal4Client(listener1)
	}

	<-end
}
