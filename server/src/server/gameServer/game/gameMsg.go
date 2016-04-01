package game

import (
	"sync"
)

type GameMsg struct {
	base_sn   int32
	config    *Config
	gameMutex *sync.RWMutex
}

func (this *GameMsg) Init(config *Config) {
	this.config = config
	this.base_sn = 100000 + config.GameId
	this.gameMutex = new(sync.RWMutex)
}

func (this *GameMsg) NoteGame(player_id int32) {

	this.gameMutex.Lock()
	defer this.gameMutex.Unlock()

	user := Role{Sn: this.base_sn + player_id, PlayerId: player_id}
	_, err := O.Insert(&user)
	if err != nil {
		Log.Error("Send error")
	}
}

func (this *GameMsg) registerGameServer(str string) {

}
