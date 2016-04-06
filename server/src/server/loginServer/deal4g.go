package main

import (
	"errors"
	"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/loginServer/account"
	"server/share/protocol"
	"sync"
)

type Connect4C struct {
	Address string
	Count   int
	Conn    net.Conn
}

type Deal4G struct {
	gameConnects map[string]Connect4C //key: "127.0.0.1:2422"
	config       *account.Config
	deal4gMutex  *sync.RWMutex
}

func (this *Deal4G) Init(config *account.Config) {
	this.gameConnects = make(map[string]Connect4C)
	this.deal4gMutex = new(sync.RWMutex)
	this.config = config
}

func (this *Deal4G) getNewAddress() (key string, value string, err error) {
	this.deal4gMutex.Lock()
	defer this.deal4gMutex.Unlock()

	var address string = ""
	var max_count int = 99999
	var address_id string = ""

	if len(this.config.NewServerAddress) == 0 {
		account.Log.Error("account.NewServerAddress len = 0")
		return "", "", errors.New("account.NewServerAddress len = 0")
	}
	for key, v := range this.config.NewServerAddress {
		if this.gameConnects[v].Count <= max_count {
			address_id = key
			address = this.gameConnects[v].Address
			max_count = this.gameConnects[v].Count
		}
	}
	return address_id, address, nil
}

func (this *Deal4G) Handler4Game(conn net.Conn) {
	//game与账号服务器断开
	defer func() {
		var key string = ""
		for _, v := range this.gameConnects {
			if v.Conn == conn {
				break
			}
		}
		delete(this.gameConnects, key)
		conn.Close()

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
				this.gameConnects[key] = Connect4C{get_account.GetGameAddress(), int(get_account.GetCount()), conn}
			}

		default:
		}
	}
}

func (this *Deal4G) Deal4GameServer(listener net.Listener) {
	for {
		conn, err := listener.Accept()
		fmt.Println(conn.RemoteAddr().String(), " connet")
		if CheckError(err) {
			go this.Handler4Game(conn)
		}
	}
}

func (this *Deal4G) NoteGame(player_id int, game_id string) error {
	pid := protocol.AccountMsgID_Msg_NoteGame
	result4G := &protocol.Account_NoteGame{
		Pid:      &pid,
		PlayerId: proto.Int32(int32(player_id)),
	}

	encObj, _ := proto.Marshal(result4G)
	server_address := this.config.NewServerAddress[game_id]

	if _, ok := this.gameConnects[server_address]; ok {
		_, err := this.gameConnects[server_address].Conn.Write(encObj)
		return err
	}
	return errors.New("game connect error")
}
