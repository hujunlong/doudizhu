package account

import ()

type LoginBase struct {
	PlayerId   int  `orm:"pk"`
	PlayerName string `orm:"size(64)"`
	PlayerPwd  string `orm:"size(64)"`
	Gold       int
	ServerId   string
	IsForBid   bool
}

type ForBid struct {
	UserId    int  `orm:"pk"`
	ForBidMsg string `orm:"size(1024)"`
}
