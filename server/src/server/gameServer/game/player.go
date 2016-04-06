package game

import (
	"github.com/game_engine/cache/redis"
	"strconv"
	"time"
)

type PlayerInfo struct { //玩家的基本信息，正常情况不会改变
	ID     int    //玩家的唯一ID
	Name   string //玩家的名字
	Gender bool   //玩家的性别
}

type Player struct {
	Info           PlayerInfo //玩家基础属性
	Day            int        //当前天数 任务重置
	DayFightNumber int        //战斗次数
	Conn           *net.Conn  //玩家的网络连接,不需要保存
}

func (this *Player) Init(config *Config) {
	this.Info.ID = 1000000 + config.GameId
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

func (this *Player) NoteGame(player_id int) {
	//检查是否已经拥有了key
	key := "role:" + strconv.Itoa(this.Info.ID+player_id)
	is_exists, _ := redis.Exists(key)

	if !is_exists {
		user := PlayerInfo{ID: this.Info.ID + player_id, Name: "", Gender: true}
		redis_err := redis.Add(key, user)
		if redis_err != nil {
			Log.Error("create role error")
		}
	}
}

func (this *Player) RegisterRole(player_id int) {
	//写入mysql 做排序
	user := Player_Rank{Sn: this.Info.ID + player_id, PlayerId: player_id}
	O.Insert(user)

	//写内存
	redis_err := redis.Add("role:"+strconv.Itoa(this.base_sn+player_id), user)
	if redis_err != nil {
		Log.Error("create role error")
	}
}

/*
func (this *Player) Save() error {
	//this.Day = time.Now().Day()
	//err := redis.Modify(this.Info.ID, this)
	//return err
}
*/
