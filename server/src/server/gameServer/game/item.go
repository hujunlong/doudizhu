//任务
package game

//"math/rand"

type ItemTemplate struct { //产生物品的模板
	Attack_Low        int32
	Attack_Low_Delta  int32
	Attack_High       int32
	Attack_High_Delta int32
	Defence           int32
	Defence_Delta     int32
	Life              int32
	Life_Delta        int32
}

type PetTemplate struct { //产生宠物的模板
	Name      string       //宠物的名字
	Attr      ItemTemplate //作为物品的基本属性
	LuckLow   int32        //最低幸运
	LuckDelta int32        //幸运变化区间
	Rate      int32        //掉落率,10000为基
}

type ItemConfig struct {
	CD        []int32  //制作物品冷却时间列表(分钟)
	Adjs      []string //物品形容词
	Objs      []string //物品名字
	Templates []ItemTemplate
}

type Item struct {
	Name       string //物品的名字
	AttackLow  int32  //物品的攻击力
	AttackHigh int32  //物品攻击力范围,实际产生的攻击为 AttackLow 到 AttackHigh 之间随机
	Defence    int32  //物品的防御力
	Life       int32  //物品的生命力
	Quality    int32  //物品的品质，决定了物品升级所需要的金钱和时间
	Value      int32  //物品的价值,创建和升级物品所投入金币的总和
}

func (this *Item) Create(name string, ic *ItemConfig, template *ItemTemplate) {

}

func (this *Item) SendMsg() {

}
