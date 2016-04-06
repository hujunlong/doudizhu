package game

import (
	"math/rand"
)

type ItemTemplate struct { //产生物品的模板
	Attack_Low        int
	Attack_Low_Delta  int
	Attack_High       int
	Attack_High_Delta int
	Defence           int
	Defence_Delta     int
	Life              int
	Life_Delta        int
}

type PetTemplate struct { //产生宠物的模板
	Name      string       //宠物的名字
	Attr      ItemTemplate //作为物品的基本属性
	LuckLow   int          //最低幸运
	LuckDelta int          //幸运变化区间
	Rate      int          //掉落率,10000为基
}

type ItemConfig struct {
	CD        []int    //制作物品冷却时间列表(分钟)
	Adjs      []string //物品形容词
	Objs      []string //物品名字
	Templates []ItemTemplate
}

type Item struct {
	Name       string //物品的名字
	AttackLow  int    //物品的攻击力
	AttackHigh int    //物品攻击力范围,实际产生的攻击为 AttackLow 到 AttackHigh 之间随机
	Defence    int    //物品的防御力
	Life       int    //物品的生命力
	Quality    int    //物品的品质，决定了物品升级所需要的金钱和时间
	Value      int    //物品的价值,创建和升级物品所投入金币的总和
}

func (this *Item) Create(name string, ic *ItemConfig, template *ItemTemplate) {
	if name != "" {
		this.Name = name
	} else {
		this.Name = ic.Adjs[rand.Intn(len(ic.Adjs))] + ic.Objs[rand.Intn(len(ic.Objs))]
	}
	if template.Attack_Low_Delta == 0 {
		this.AttackLow = 0
	} else {
		this.AttackLow = template.Attack_Low + rand.Intn(template.Attack_Low_Delta)
	}
	if template.Attack_High_Delta == 0 {
		this.AttackHigh = 0
	} else {
		this.AttackHigh = template.Attack_High + rand.Intn(template.Attack_High_Delta)
	}
	if this.AttackHigh < this.AttackLow {
		this.AttackHigh, this.AttackLow = this.AttackLow, this.AttackHigh
	}
	if template.Life_Delta == 0 {
		this.Life = 0
	} else {
		this.Life = template.Life + rand.Intn(template.Life_Delta)
	}
	if template.Defence_Delta == 0 {
		this.Defence = 0
	} else {
		this.Defence = template.Defence + rand.Intn(template.Defence_Delta)
	}
	this.Value = 0
	this.Quality = rand.Intn(100)
}

func (this *Item)  SendMsg(){
	
}