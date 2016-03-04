package main

import (
	"github.com/game_engine/logs"
	"github.com/golang/protobuf/proto"
	"server/share/global"
	"server/share/protocol"
)

import (
	"fmt"
	"net"
	"strconv"
	"time"
)

var log *logs.BeeLogger

const max_client = 20000

var end = make(chan int)

func init() {
	log = logs.NewLogger(100000) //日志
	log.EnableFuncCallDepth(true)
	log.SetLogger("file", `{"filename":"testLoginServer.log"}`)
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err)
		return false
	}
	return true
}

func SendMsgRegister(conn net.Conn, i int) {
	nick := strconv.Itoa(i)
	register := &protocol.S2SSystem_RegisterPlayer{
		Pid:        proto.Int32(global.RegisterInfoId),
		Playername: proto.String(nick),
		Passworld:  proto.String(nick),
	}

	encObj, err := proto.Marshal(register)
	is_ok := CheckError(err)
	if is_ok {
		conn.Write(encObj)
	}
}

func SenMsgLogin(conn net.Conn, i int) {
	nick := strconv.Itoa(i)
	//登陆相关
	loginInfo := &protocol.S2SSystem_LoginInfo{
		Pid:        proto.Int32(global.LoginInfoId),
		Playername: proto.String(nick),
		Passworld:  proto.String(nick),
	}

	encObj, err := proto.Marshal(loginInfo)
	is_ok := CheckError(err)
	if is_ok {
		conn.Write(encObj)
	}
}

func ReciveResult(conn net.Conn, i int, recive_result chan int) {
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)

	for true {
		n, _ := conn.Read(buf) //接收具体消息
		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.S2SSystem_GetType)
		if err := proto.Unmarshal(buf[0:n], typeStruct); err != nil {
			CheckError(err)
		}

		fmt.Println("*typeStruct.Pid=", *typeStruct.Pid)
		switch *typeStruct.Pid {
		case global.LoginResultId:
			result := new(protocol.S2SSystem_LoginResult)
			if err := proto.Unmarshal(buf[0:n], result); err == nil {
				switch result.GetResult() {
				case global.LOGINSUCCESS:
					log.Info("login sucessfull")
				default:
					log.Error("login error")
				}

				conn.Close()
				log.Info("close connect")
				recive_result <- 1
				if i == max_client-1 {
					end <- 1
				}
				return
			}
		case global.RegisterResultId:
			result := new(protocol.S2SSystem_RegisterResult)
			if err := proto.Unmarshal(buf[0:n], result); err == nil {
				switch result.GetResult() {
				case global.REGISTERSUCCESS:
					log.Trace("register sccessfull!")
				default:
					log.Error("register error")
				}
				//注册后登陆
				SenMsgLogin(conn, i)
			}
			break
		}
		time.Sleep(5 * time.Millisecond)
	}
}

func MessageRun(conn net.Conn, i int) {
	//通信先获取返回数据
	result := make(chan int)
	go ReciveResult(conn, i, result)
	SendMsgRegister(conn, i)
	<-result
}

type ConnStruct struct {
	conn net.Conn
}

func main() {
	var arrayConnStruct [max_client]ConnStruct
	var err error
	for i := 0; i < max_client; i++ {
		arrayConnStruct[i].conn, err = net.Dial("tcp", "127.0.0.1:8080")
		if err != nil {
			log.Error("connect error %s", err)
			return
		}
		go MessageRun(arrayConnStruct[i].conn, i)
		time.Sleep(5 * time.Millisecond)
	}
	<-end
}
