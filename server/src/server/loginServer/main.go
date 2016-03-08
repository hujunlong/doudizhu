package main

import (
	"github.com/golang/protobuf/proto"
	"server/loginServer/account"
	"server/share/global"
	"server/share/protocol"
)

import (
	"errors"
	"fmt"
	"net"
)

var gameConnects map[string]account.Connect4C

func init() {
	gameConnects = make(map[string]account.Connect4C)
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err.Error())
		return false
	}
	return true
}

func getNewAddress() (string, error) {
	var address string
	var max_count int32 = 99999
	if len(account.NewServerAddress) == 0 {
		account.Log.Error("account.NewServerAddress len = 0")
		return "", errors.New("account.NewServerAddress len = 0")
	}
	for _, v := range account.NewServerAddress {
		if gameConnects[v].Count <= max_count {
			address = gameConnects[v].Address
			max_count = gameConnects[v].Count
		}
	}
	return address, nil
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
			account.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.Account_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case global.LoginInfoId:
			//登陆
			login := new(protocol.Account_LoginInfo)
			if err := proto.Unmarshal(buf[0:n], login); err == nil {
				result, player_id := account.VerifyLogin(login.GetPlayername(), login.GetPassworld())
				//发送登陆并断开连接
				result4C := &protocol.Account_LoginResult{
					Pid:      proto.Int32(global.LoginResultId),
					Result:   proto.Int32(result),
					PlayerId: proto.Int32(player_id),
				}

				gameserver := ""
				if result == global.LOGINSUCCESS {
					gameserver, _ = getNewAddress()
				}

				result4C.Gameserver = &(gameserver)
				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				conn.Close()
				account.Log.Info("send login message")
			}
		case global.RegisterInfoId:
			//注册
			register := new(protocol.Account_RegisterPlayer)
			if err := proto.Unmarshal(buf[0:n], register); err == nil {
				result := account.Register(register.GetPlayername(), register.GetPassworld())

				result4C := &protocol.Account_RegisterResult{
					Pid:    proto.Int32(global.RegisterResultId),
					Result: proto.Int32(result),
				}

				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				account.Log.Info("send register message")
			}
		case global.GameResultId:
			//读取各个服务器发送过来的在线人数
			get_account := new(protocol.Account_GameResult)
			if err := proto.Unmarshal(buf[0:n], get_account); err == nil {
				key := get_account.GetGameAddress()
				gameConnects[key] = account.Connect4C{get_account.GetGameAddress(), get_account.GetCount()}
				account.Log.Info("get account message")
			}
		default:
		}
	}
}

func Handler4Game(conn net.Conn) {
	defer func() {
		conn.Close()
		delete(gameConnects, conn.RemoteAddr().String())
	}()

	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)
	for {
		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN {
			account.Log.Error("recive error n> MAXLEN")
			return
		}

	}
}

func Deal4Client(listener net.Listener) {
	conn, err := listener.Accept()
	if CheckError(err) {
		go Handler4C(conn)
	}
}

func Deal4GameServer(listener net.Listener) {
	conn, err := listener.Accept()
	key_value := conn.RemoteAddr().String()
	fmt.Println(key_value)
	if CheckError(err) {
		gameConnects[key_value] = account.Connect4C{
			key_value,
			0,
		}
		go Handler4Game(conn)
	}
}

func main() {

	//Deal4Client
	listener, err := net.Listen("tcp", account.Listen4CAddress)

	//Deal4Game
	listener2, err2 := net.Listen("tcp", account.Listen4GameAddress)

	if !CheckError(err) || !CheckError(err2) {
		return
	}

	for {
		Deal4Client(listener)
		Deal4GameServer(listener2)
	}

}
