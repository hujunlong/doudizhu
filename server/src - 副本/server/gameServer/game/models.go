package game

type BaseInfo struct {
	Name string
	Sex  int32
}

type Equip struct {
	EquipsId []int32
}

type Item struct {
	ItemId []int32
}

type Role struct {
	Sn       int32 `orm:"pk"`
	PlayerId int32
	BaseInfo string
	Equip    string
	Item     string
}
