#pragma once
#include "cocos2d.h"
#include "GameLayer.h"
#include "NetGameLayer.h"
USING_NS_CC;

//选择单机 网络类型
class ChoosePlayTypeLayer : public CCLayer
{
public:
	ChoosePlayTypeLayer(void);
	~ChoosePlayTypeLayer(void);
	virtual bool init();
	void menuAloneHandle(CCObject* sender);//单机
	void menuNetHandle(CCObject* sender);//网络

	CREATE_FUNC(ChoosePlayTypeLayer);

private: 
	Menu* m_handle_menu;//选择菜单
};