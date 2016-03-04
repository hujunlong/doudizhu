#pragma once
#include "cocos2d.h"
#include "Poker.h"
USING_NS_CC;

//手中发牌后牌型组合
struct PkHandType
{
	CARD_TYPE type;
	std::vector<Poker*> vec_poker;
};


class  Player : public CCObject
{
public:
	 Player();
	~ Player();
	void updatePkPosion();//设置牌的位

	public:
	std::vector<PkHandType> m_vec_types;
private:
	CC_SYNTHESIZE(bool,m_isDizhu,IsDiZhu);//是否为地主
	CC_SYNTHESIZE(bool,m_isCall,Call);//是否已经叫地主了
	CC_SYNTHESIZE(int,m_CallScore,CallScore);//地主叫分
	CC_SYNTHESIZE(CCArray*,m_arrPk,ArrPK);//手中的牌
	CC_SYNTHESIZE(CCPoint,m_startLocation,StartLocation);//牌在手中的初始位置
	CC_SYNTHESIZE(int,m_HandleType,HandleType);
	CC_SYNTHESIZE(bool,m_isOutPk,IsOutPk);//上轮是否最后接牌
 
};

 