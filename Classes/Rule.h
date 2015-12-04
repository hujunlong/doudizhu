//斗地主游戏规则
#pragma once

#include "cocos2d.h"
#include "Global.h"
#include <vector>

USING_NS_CC;
using namespace  std;

//npc AI 以及 游戏牌型判断
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
	void AiSplitPks(vector<PkStruct> vec_pks);//分析牌型
	int PlayerRule(vector<PkStruct> my_vec_pks,PkHandleTypeStruct pk_handle_type_struct);//检查是否合符规则
	//判断是否合符规则
	bool IsFindPk(PkStruct pk,std::vector<PkHandleTypeStruct> pk_handle);//查找是否有该pk
	bool IsPlane(vector<PkStruct> vec_pks);//是否为飞机
	bool IsBomb(vector<PkStruct> vec_pks);//是否为炸弹出牌规则
	bool IsThreeAddNoPk(vector<PkStruct> vec_pks);//是否为三不带
	bool IsThreeAddOnePk(vector<PkStruct> vec_pks);//是否为三带一
	bool IsThreeAddDoublePk(vector<PkStruct> vec_pks);//是否为三带一对
	bool IsSingleShunzhiPk(vector<PkStruct> vec_pks);//单顺
	bool IsDoubleShunzhiPk(vector<PkStruct> vec_pks);//双顺
	bool IsThreeShunZhi(vector<PkStruct> vec_pks);//三顺
	bool IsPlaneAddSingleWings(vector<PkStruct> vec_pks);//飞机带翅膀
	bool IsPlaneAddDoubleWings(vector<PkStruct> vec_pks);//飞机带翅膀
	bool BombAddSinglePk(vector<PkStruct> vec_pks);//四带二单牌
	bool BombAddDoublePk(vector<PkStruct> vec_pks);//四带二双牌
private:
	int Plane(vector<PkStruct> pData);//飞机有1还是0
	int Bomb(vector<PkStruct> pData);//炸弹有几个
	int GetCallBigPks(vector<PkStruct> pData);//获取王跟2的叫牌分数
	static bool SelectShort(const PkStruct& p1,const PkStruct& p2);

	std::vector<PkStruct> AiThreePks(vector<PkHandleTypeStruct> vec_pk_handle_struct_,std::vector<PkStruct> &vec_pks);//提取三顺
	std::vector<PkStruct> AiSingleShunzhi(std::vector<PkStruct> &vec_pks);//提取顺子
	void AiLianDui();//提取连队
	void GetOneAiLianDui();//获取其中的一个连对
	std::vector<PkStruct> AiDouble(std::vector<PkStruct> &vec_pks);//提取对子
	void AiSingle(std::vector<PkStruct> &vec_pks);//单牌获取
	void AiHandPksType(std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct);//出手牌型

public:
	std::vector<PkStruct> pks;//所有牌
	std::vector<PkStruct> pks_player;
	std::vector<PkStruct> pks_npc1;
	std::vector<PkStruct> pks_npc2;
	std::vector<PkStruct> pks_three;
	std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct;//分析后的牌型
	std::vector<PkHandleTypeStruct> vec_pk_hands;//要出的牌
};
