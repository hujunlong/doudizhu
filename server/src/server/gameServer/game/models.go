//数据库模型
package game

//game中的player是游戏中的角色
type Player_Rank struct {
	ID       int32 `orm:"pk"`
	PlayerId int32
	ServerId int32
	Gold     int32
}
