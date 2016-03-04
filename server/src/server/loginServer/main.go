package main

import (
	"github.com/golang/protobuf/proto"
	"server/loginServer/account"
	"server/share/global"
	"server/share/protocol"
)

import (
	"fmt"
	"io"
	"net"
)

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func Handler(conn net.Conn) {
	defer conn.Close()
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)
	for {
		n, err := conn.Read(buf) //接收具体消息
		if err == io.EOF {
			fmt.Println(conn.RemoteAddr(), " closed")
			conn.Close()
			return
		} else if !CheckError(err) {
			return
		}

		if n > MAXLEN {
			account.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.S2SSystem_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case global.LoginInfoId:
			//登陆
			login := new(protocol.S2SSystem_LoginInfo)
			if err := proto.Unmarshal(buf[0:n], login); err == nil {
				result := account.VerifyLogin(login.GetPlayername(), login.GetPassworld())

				//发送登陆blanceServer地址并断开连接
				result4C := &protocol.S2SSystem_LoginResult{
					Pid:    proto.Int32(global.LoginResultId),
					Result: proto.Int32(result),
				}

				balanceserver := ""
				if result == global.LOGINSUCCESS {
					balanceserver = account.BlanceAddress
				}
				result4C.Balanceserver = &(balanceserver)
				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				conn.Close()
				account.Log.Info("send login message")
			}
		case global.RegisterInfoId:
			//注册
			register := new(protocol.S2SSystem_RegisterPlayer)
			if err := proto.Unmarshal(buf[0:n], register); err == nil {
				result := account.Register(register.GetPlayername(), register.GetPassworld())

				result4C := &protocol.S2SSystem_RegisterResult{
					Pid:    proto.Int32(global.RegisterResultId),
					Result: proto.Int32(result),
				}

				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				account.Log.Info("send register message")
			}
		default:
		}
	}
}

func main() {
	listener, err := net.Listen("tcp", account.ListenAddress)
	if CheckError(err) {
		for {
			conn, err1 := listener.Accept()
			if CheckError(err1) {
				go Handler(conn)
			}
		}
	}
}
