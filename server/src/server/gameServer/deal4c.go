package main

import (
	"fmt"
	"github.com/golang/protobuf/proto"
	"net"
	"server/gameServer/game"
	"server/share/global"
	"server/share/protocol"
	"strconv"
)

type Deal4C struct {
	server_id int
	players   map[string]*game.Player //在线玩家指针 map
}

func (this *Deal4C) Init(server_id int) {
	this.server_id = server_id
	this.players = make(map[string]*game.Player)
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

	var player *game.Player //玩家对象的指针

	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)
	for {
		fmt.Println(buf)
		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN {
			game.Log.Error("recive error n> MAXLEN")
			return
		}

		//接收包的type类型用来区分包之间的区别
		typeStruct := new(protocol.Game_GetType)
		if err1 := proto.Unmarshal(buf[0:n], typeStruct); err1 != nil {
			CheckError(err1)
			continue
		}

		switch *typeStruct.Pid {
		case protocol.GameMsgID_Game_Msg_RegisterRole: //注册
			register := new(protocol.Game_RegisterRole)
			if err := proto.Unmarshal(buf[0:n], register); err == nil {
				player = new(game.Player)
				player.Init(&conn)
				//1000000与playerid区分明显
				sn := 1000000 + this.server_id + int(register.Info.GetPlayerId())
				result := player.RegisterRole(sn, int(register.Info.GetPlayerId()), this.server_id, register.Info.GetNick(), register.Info.GetGender())

				pid := protocol.GameMsgID_Game_Msg_RegisterRoleResult
				result4C := &protocol.Game_RegisterRoleResult{
					Pid:    &pid,
					Result: proto.Int32(int32(result)),
				}
				encObj, _ := proto.Marshal(result4C)
				conn.Write(encObj)

				//加载保存于内存中
				if global.REGISTERROLESUCCESS == result {
					key := strconv.Itoa(sn)
					this.players[key] = player
				}
			}

		case protocol.GameMsgID_Game_Msg_GetRoleInfo: //获取role基础属性
			//result4C = pl
			//encObj, _ := proto.Marshal(result4C)
			//conn.Write(encObj)
		default:
		}
	}
}
