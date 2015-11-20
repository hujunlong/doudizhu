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
	void GiveDizhuPks(int type);//给地主

	int Call(int index);//传入npc是第几个
	void AiSplitPks(int type);//分析牌型
	
private:
	int Plane(vector<PkStruct> pData);//飞机有1还是0
	int Bomb(vector<PkStruct> pData);//炸弹有几个
	int GetCallBigPks(vector<PkStruct> pData);//获取王跟2的叫牌分数
	static bool SelectShort(const PkStruct& p1,const PkStruct& p2);

	std::vector<PkStruct> AiThreePks(vector<PkHandleTypeStruct> vec_pk_handle_struct_,std::vector<PkStruct> &vec_pks);//提取三顺
	std::vector<PkStruct> AiSingleShunzhi(std::vector<PkStruct> &vec_pks);//提取顺子
	std::vector<PkStruct> AiDouble(std::vector<PkStruct> &vec_pks);//提取对子
	void AiSingle(std::vector<PkStruct> &vec_pks);//单牌获取

public:
	PkStruct pks[54];
	std::vector<PkStruct> pks_player;
	std::vector<PkStruct> pks_npc1;
	std::vector<PkStruct> pks_npc2;
	std::vector<PkStruct> pks_three;
	std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct;//分析后的牌型
};
