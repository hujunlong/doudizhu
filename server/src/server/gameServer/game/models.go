package game

//game中的player是游戏中的角色 可多角色开
type Player_Rank struct {
	Sn       int `orm:"pk"`
	PlayerId int
	Gold     int
}
