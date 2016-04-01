package game

import (
	"github.com/game_engine/cache/redis"
	"strconv"
)

type GameMsg struct {
	base_sn int32
	config  *Config
}

func (this *GameMsg) Init(config *Config) {
	this.config = config
	this.base_sn = 100000 + config.GameId
}

func (this *GameMsg) NoteGame(player_id int32) {

	user := Role{Sn: this.base_sn + player_id, PlayerId: player_id}
	var sn_int int = int(this.base_sn + player_id)

	redis_err := redis.Add("role:"+strconv.Itoa(sn_int), user)
	if redis_err != nil {
		Log.Error("create role error")
	}
}

func (this *GameMsg) registerGameServer(str string) {

}
