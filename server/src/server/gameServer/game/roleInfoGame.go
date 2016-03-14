package game

import (
	"fmt"
)

var base_sn int32 = 100000 + game_id

func NoteGame(player_id int32) {
	gameMutex.Lock()
	defer gameMutex.Unlock()

	user := Role{Sn: base_sn + player_id, PlayerId: player_id}
	_, err := O.Insert(&user)
	if err != nil {
		fmt.Println("insert err :", err)
	}
}

func registerGameServer(str string) {

}
