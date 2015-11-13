//斗地主游戏规则
#pragma once

#include "cocos2d.h"

#include "Global.h"
#include <vector>

USING_NS_CC;
using namespace  std;

struct PkHandleTypeStruct//出牌类型
{
	CARD_TYPE type;
	std::vector<PkStruct> pk_structs;
};

class Rule 
{
public:
	Rule();
	~Rule();
	void InitPks();//初始牌
	void Shuffle();//洗牌
	
	int Call(int index);//传入npc是第几个
	int Plane(vector<PkStruct> pData);//飞机有1还是0
	int Bomb(vector<PkStruct> pData);//炸弹有几个
	int GetCallBigPks(vector<PkStruct> pData);//获取王跟2的叫牌分数
	static bool SelectShort(const PkStruct& p1,const PkStruct& p2);
	void AiSplitPks(std::vector<PkStruct> &pks);//第几个玩家
	void AiThreePks(std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct_buff);//提取三顺


	bool IsPlane(CCArray *arr);//是否为飞机
	bool IsBomb(CCArray *arr);//是否为炸弹
	bool IsDouble(CCArray *arr);//是否为双
	bool IsThreePk(CCArray *arr);//三条
	bool IsThreePkAddOne(CCArray *arr);//三带一
	bool IsSingleShunzhi(CCArray *arr);//单顺
	bool IsDoubleShunzhi(CCArray *arr);//双顺
	bool IsThreeShunzhi(CCArray *arr);//三顺
	bool IsPlaneAddWings(CCArray *arr);//飞机带翅膀
	bool BombAddTwo(CCArray *arr);//四带2

	public:
	 PkStruct pks[54];
	 std::vector<PkStruct> pks_player;
	 std::vector<PkStruct> pks_npc1;
	 std::vector<PkStruct> pks_npc2;
	 std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct;
};
