package main

import (
	"errors"
	"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/loginServer/account"
	"server/share/protocol"
)

func getNewAddress() (key string, value string, err error) {
	accountMutex.Lock()
	defer accountMutex.Unlock()

	var address string = ""
	var max_count int32 = 99999
	var address_id string = ""
	if len(account.NewServerAddress) == 0 {
		account.Log.Error("account.NewServerAddress len = 0")
		return "", "", errors.New("account.NewServerAddress len = 0")
	}
	for key, v := range account.NewServerAddress {
		if gameConnects[v].Count <= max_count {
			address_id = key
			address = gameConnects[v].Address
			max_count = gameConnects[v].Count
		}

	}
	return address_id, address, nil
}

func Handler4Game(conn net.Conn) {
	//game与账号服务器断开
	defer func() {
		var key string = ""
		for k, v := range gameConnects {
			if v.Conn == conn {
				key = k
				fmt.Println(key, v)
				break
			}
		}
		delete(gameConnects, key)
		conn.Close()

		fmt.Println("come here now gameConnects.len = ", len(gameConnects), "key = ", key)

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

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.Account_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case protocol.AccountMsgID_Msg_GameResult:
			//读取各个服务器发送过来的在线人数
			get_account := new(protocol.Account_GameResult)
			if err := proto.Unmarshal(buf[0:n], get_account); err == nil {
				key := get_account.GetGameAddress()
				gameConnects[key] = account.Connect4C{get_account.GetGameAddress(), get_account.GetCount(), conn}
				account.Log.Info("get account message")
			}

		default:
		}
	}

}

func Deal4GameServer(listener net.Listener) {
	for {
		conn, err := listener.Accept()
		if CheckError(err) {
			go Handler4Game(conn)
		}
	}

}
