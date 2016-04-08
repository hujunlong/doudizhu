package game

//game中的player是游戏中的角色 可多角色开
type Player_Rank struct {
	ID       int `orm:"pk"`
	PlayerId int
	ServerId int
	Gold     int
}
