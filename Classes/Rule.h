//斗地主游戏规则
#pragma once

#include "Global.h"
#include <vector>
using namespace  std;

struct PkStruct //牌 类型 大小
{
	int pk_type;
	int pk_num;
};

class Rule 
{
public:
	Rule();
	~Rule();
	void InitPks();//初始牌
	void Shuffle();//洗牌
	void SelectShort(PkStruct* pData,int count);
	int Call(int index);//传入npc是第几个
	int Plane(PkStruct* pData,int count);
	int Bomb(PkStruct* pData,int count);
	int GetCallBigPks(PkStruct* pData,int count);//获取王跟2的叫牌分数
	public:
	 PkStruct pks[54];
	 PkStruct pks_player[17];
	 PkStruct pks_npc1[17];
	 PkStruct pks_npc2[17];
};
