//角色
package game

import (
	"net"
	"server/share/global"
	"strconv"
	"time"

	"github.com/game_engine/cache/redis"
)

type PlayerInfo struct { //角色的基本信息，正常情况不会改变
	ID              int32   //角色的唯一ID
	Nick            string  //角色的名字
	RoleId          int32   //配置表中的id
	Level           int32   //等级
	Exp             int32   //经验
	Hp              int32   //血量
	Physical_def    []int32 //物理防御(升级 ,强化,精炼)
	Magic_def       []int32 //法术防御
	Physical_damage []int32 //物理伤害
	Magic_damage    []int32 //法术伤害
	Equip           []int32 //装备
}

type Player struct {
	Info           PlayerInfo //角色基础属性
	Day            int        //当前天数 任务重置
	DayFightNumber int32      //战斗次数
	Conn           *net.Conn  //角色的网络连接,不需要保存
}

func (this *Player) Init(conn *net.Conn) {
	this.Conn = conn
}

func LoadPlayer(id string) *Player { //从数据库读取玩家信息
	player := new(Player)
	err := redis.Find(id, player)
	if err == nil {
		if time.Now().Day() != int(player.Day) {
			player.DayFightNumber = 0
		}
		return player
	}
	return nil
}

func (this *Player) RegisterRole(id int32, player_id int32, server_id int32, nick string, HeroId int32) int32 {

	//检查是否已经拥有了key
	key := "role:" + strconv.Itoa(int(id))
	is_exists, _ := redis.Exists(key)

	//检测heroid是否在配置中
	HeroId_str := strconv.Itoa(int(HeroId))
	if Csv.Create_role[HeroId_str] == nil {
		return global.REGISTERROLEERROR
	}

	if is_exists {
		this.Day = 0
		this.DayFightNumber = 0
		this.Info.ID = id
		this.Info.Nick = nick
		this.Info.RoleId = HeroId

		//写入mysql 做排序
		//user := Player_Rank{ID: id, PlayerId: player_id, ServerId: server_id, Gold: 0}
		//O.Insert(user)

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
	err := redis.Modify("role:"+strconv.Itoa(int(this.Info.ID)), this)
	return err
}
