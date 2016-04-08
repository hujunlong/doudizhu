package game

import (
	"github.com/game_engine/cache/redis"
	"net"
	"server/share/global"
	"strconv"
	"time"
)

type PlayerInfo struct { //角色的基本信息，正常情况不会改变
	ID     int    //角色的唯一ID
	Name   string //角色的名字
	Gender bool   //角色的性别
}

type Player struct {
	Info           PlayerInfo //角色基础属性
	Day            int        //当前天数 任务重置
	DayFightNumber int        //战斗次数
	Conn           *net.Conn  //角色的网络连接,不需要保存
}

func (this *Player) Init(conn *net.Conn) {
	this.Conn = conn
}

func LoadPlayer(id string) *Player { //从数据库读取玩家信息
	player := new(Player)
	err := redis.Find(id, player)
	if err == nil {
		if time.Now().Day() != player.Day {
			player.DayFightNumber = 0
		}
		return player
	}
	return nil
}

func (this *Player) RegisterRole(id int, player_id int, server_id int, name string, gender bool) int {
	//检查是否已经拥有了key
	key := "role:" + strconv.Itoa(id)
	is_exists, _ := redis.Exists(key)

	if is_exists {

		this.Day = 0
		this.DayFightNumber = 0
		this.Info.ID = id
		this.Info.Name = name
		this.Info.Gender = gender

		//写入mysql 做排序
		user := Player_Rank{ID: id, PlayerId: player_id, ServerId: server_id, Gold: 0}
		O.Insert(user)

		//写内存数据库
		this.Save()
		return global.REGISTERROLESUCCESS
	}
	return global.REGISTERROLEERROR
}

func (this *Player) GetRoleInfo() {

}

func (this *Player) Save() error {
	this.Day = time.Now().Day()
	err := redis.Modify("role:"+strconv.Itoa(this.Info.ID), this)
	return err
}
