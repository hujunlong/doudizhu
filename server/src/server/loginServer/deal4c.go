package main

import (
	"github.com/golang/protobuf/proto"
	"net"
	"server/loginServer/account"
	"server/share/global"
	"server/share/protocol"
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
		case protocol.AccountMsgID_Msg_LoginInfo:
			//登陆
			login := new(protocol.Account_LoginInfo)
			if err := proto.Unmarshal(buf[0:n], login); err == nil {
				result, player_id, server_address := this.account_info.VerifyLogin(login.GetPlayername(), login.GetPassworld())

				//发送登陆并断开连接
				pid := protocol.AccountMsgID_Msg_LoginResult
				result4C := &protocol.Account_LoginResult{
					Pid:        &pid,
					Result:     proto.Int32(int32(result)),
					PlayerId:   proto.Int32(int32(player_id)),
					Gameserver: proto.String(server_address),
				}

				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				conn.Close()
			}

		case protocol.AccountMsgID_Msg_RegisterPlayer:
			//注册
			register := new(protocol.Account_RegisterPlayer)
			if err := proto.Unmarshal(buf[0:n], register); err == nil {
				game_id, _, _ := this.deal4g.getNewAddress()
				result, player_id := this.account_info.Register(register.GetPlayername(), register.GetPassworld(), game_id)

				pid := protocol.AccountMsgID_Msg_RegisterResult
				result4C := &protocol.Account_RegisterResult{
					Pid:    &pid,
					Result: proto.Int32(int32(result)),
				}

				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)
				account.Log.Info("send register message")

				//通知game注册成功

				if global.REGISTERSUCCESS == result {
					err := this.deal4g.NoteGame(player_id, game_id)
					if err != nil {
						account.Log.Error("err:", err)
					}
					account.Log.Info("player_id = %d game_id=%d", player_id, game_id)
				}

			}

		default:
		}
	}
}
