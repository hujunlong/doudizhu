package main

import (
	"fmt"
	"net"
	"server/gameServer/game"
	"server/share/global"
	"server/share/protocol"
	"strconv"

	"github.com/golang/protobuf/proto"
)

var sys_config *game.SysConfig //系统配置

type Deal4C struct {
	server_id int32
	players   map[string]*game.Player //在线玩家指针 map

}

func (this *Deal4C) Init() {
	sys_config = new(game.SysConfig)
	sys_config.Init()

	this.server_id = sys_config.GameId
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

		n, err := conn.Read(buf) //接收具体消息
		if err != nil {
			return
		}

		if n > MAXLEN {
			game.Log.Error("recive error n> MAXLEN")
			return
		}

		fmt.Println(buf[0:n])

		//接收包头
		_, head_pid := GetHead(buf)
		//包体
		switch head_pid {
		case 1001: //注册
			register := new(protocol.Game_RegisterRole)
			if err := proto.Unmarshal(buf[8:n], register); err == nil {
				player = new(game.Player)
				player.Init(&conn)
				sn := this.server_id + register.Info.GetPlayerId()

				fmt.Println(register.Info.GetPlayerId(), this.server_id, register.Info.GetNick(), register.Info.GetHeroId())
				result := player.RegisterRole(sn, register.Info.GetPlayerId(), this.server_id, register.Info.GetNick(), register.Info.GetHeroId())

				result4C := &protocol.Game_RegisterRoleResult{
					Result: proto.Int32(result),
				}
				encObj, _ := proto.Marshal(result4C)
				SendPackage(conn, 1001, encObj)
				//加载保存于内存中
				if global.REGISTERROLESUCCESS == result {
					key := strconv.Itoa(int(sn))
					this.players[key] = player
				}
			}

		case 1002: //获取role基础属性

			fmt.Println("come role base")
			var pid int32 = 123
			var nick string = "afds"
			var hero_id int32 = 12
			//var level int32 = 1
			var exp int32 = 0
			var hp int32 = 70
			physical_def := []int32{2, 4}
			magic_def := []int32{2, 4}
			physical_damage := []int32{2, 4}
			magic_damage := []int32{2, 4}

			result4C := &protocol.Game_RoleInfoResult{

				BaseRole: &protocol.Game_BaseRole{
					PlayerId: &pid,
					Nick:     &nick,
					HeroId:   &hero_id,
				},

				Attribute: &protocol.Game_Attribute{
					Exp:            &exp,
					Hp:             &hp,
					PhysicalDef:    physical_def,
					MagicDef:       magic_def,
					PhysicalDamage: physical_damage,
					MagicDamage:    magic_damage,
				},
			}

			encObj, _ := proto.Marshal(result4C)
			fmt.Println(encObj)
			SendPackage(conn, 1002, encObj)

		default:
		}
	}
}
