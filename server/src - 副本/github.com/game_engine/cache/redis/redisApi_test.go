package redis

import (
	"testing"
)

type PlayerInfo struct {
	ID   string
	Name string
	Age  int32
	Sex  int32
}

func showError(t *testing.T, err error) {
	if err != nil {
		t.Error(err)
	}
}
func TestRedisApi(t *testing.T) {
	player_info := PlayerInfo{"1111", "zhang", 12, 1}
	player_info2 := new(PlayerInfo)

	//add
	err := Add("1111", player_info)
	showError(t, err)

	//find
	err = Find("1111", player_info2)
	showError(t, err)

	//modify
	player_info = PlayerInfo{"2222", "zhang", 12, 1}
	err = Modify("1111", player_info)
	showError(t, err)

	//find
	err = Find("1111", player_info2)
	showError(t, err)
	if player_info2.ID != "2222" {
		t.Fatal("modify erro")
	}

	//del
	ok, err2 := Del("1111")
	showError(t, err2)
	if !ok {
		t.Fatal(err2)
	}
}
