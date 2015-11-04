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

	public:
	 PkStruct pks[54];
};
