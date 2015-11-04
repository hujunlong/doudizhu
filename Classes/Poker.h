#pragma once

#include "cocos2d.h"
#include "Global.h"
class GameLayer;

USING_NS_CC;

class Poker : public Sprite
{
public:
	Poker();
	~Poker();

	static Poker* create(const char *poker_name,const CCRect& rect);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void showFront();//显示正面
	void showBack();//显示背面
	Poker* copy(); 
	void setTouchPriority(int num);//设置触摸优先级
	void selectPkTop();//选中上移
	void selectPkDown();//选中下移


private:
	CC_SYNTHESIZE(bool,m_isSelect,Select);//CC_SYNTHESIZE 宏让private可以外部访问 很奇特
	CC_SYNTHESIZE(GameLayer*,m_gameMain,GameMain);
	CC_SYNTHESIZE(bool,m_isClick,Click);
	CC_SYNTHESIZE(int,m_PkType,PkType);
	CC_SYNTHESIZE(int,m_PkNum,PkNum);

	EventListenerTouchOneByOne* touchListener;
};

 