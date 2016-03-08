package game

type RoleInfo struct {
	Sn       int32 `orm:"pk"`
	PlayerId int32
	NickName string `orm:"size(64)"`
	Sex      int32
}
