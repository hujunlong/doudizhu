package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"server/loginServer/account"
	"server/share/global"
	"server/share/protocol"

	"github.com/golang/protobuf/proto"
)

type Deal4C struct {
	deal4g       *Deal4G
	account_info *account.AccountInfo
}

func (this *Deal4C) Init(account_info *account.AccountInfo, deal4g *Deal4G) {
	this.account_info = account_info
	this.deal4g = deal4g
}

func (this *Deal4C) Deal4Client(listener net.Listener) {
	for {
		conn, err := listener.Accept()
		fmt.Println(conn.RemoteAddr(), "connected")
		if CheckError(err) {
			go this.Handler4C(conn)
		}
	}
}

func (this *Deal4C) Handler4C(conn net.Conn) {
	defer conn.Close()
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)

	for {
		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN && n < 8 {
			account.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包头
		var head_len int32 = 0
		var head_pid int32 = 0
		buffer_len := bytes.NewBuffer(buf[0:4])
		buffer_pid := bytes.NewBuffer(buf[4:8])
		binary.Read(buffer_len, binary.BigEndian, &head_len)
		binary.Read(buffer_pid, binary.BigEndian, &head_pid)

		//接收包体
		switch head_pid {

		case 1:
			//注册
			register := new(protocol.Account_RegisterPlayer)
			if err := proto.Unmarshal(buf[8:n], register); err == nil {
				game_id, _, _ := this.deal4g.getNewAddress()
				fmt.Println(register.GetPlayername(), register.GetPassworld())
				result, player_id := this.account_info.Register(register.GetPlayername(), register.GetPassworld(), game_id)

				result4C := &protocol.Account_RegisterResult{
					Result: proto.Int32(int32(result)),
				}

				encObj, _ := proto.Marshal(result4C)
				SendPackage(conn, 1, encObj)

				//通知game注册成功
				if global.REGISTERSUCCESS == result {
					err := this.deal4g.NoteGame(player_id, game_id)
					if err != nil {
						account.Log.Error("err:", err)
					}
					account.Log.Info("player_id = %d game_id=%d", player_id, game_id)
				}

			}

		case 2:
			//登陆
			login := new(protocol.Account_LoginInfo)
			if err := proto.Unmarshal(buf[8:n], login); err == nil {
				result, player_id, server_address := this.account_info.VerifyLogin(login.GetPlayername(), login.GetPassworld())
				//发送登陆并断开连接
				result4C := &protocol.Account_LoginResult{
					Result:     proto.Int32(int32(result)),
					PlayerId:   proto.Int32(int32(player_id)),
					Gameserver: proto.String(server_address),
				}

				encObj, _ := proto.Marshal(result4C)
				SendPackage(conn, 2, encObj)
				conn.Close()
			} else {
				fmt.Println(err)
			}

		default:
		}
	}
}
