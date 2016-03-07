package account

import ()

type LoginBase struct {
	PlayerId   int32  `orm:"pk"`
	PlayerName string `orm:"size(64)"`
	PlayerPwd  string `orm:"size(64)"`
	Gold       int32
	ServerId   int32
	IsForBid   bool
}

type ForBid struct {
	UserId    int32  `orm:"pk"`
	ForBidMsg string `orm:"size(1024)"`
}

type Connect4C struct {
	Address string
	Count   int32
}
