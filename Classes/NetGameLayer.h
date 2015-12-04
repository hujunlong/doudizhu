#pragma once
#include "cocos2d.h"
#include "MySocket.h"

USING_NS_CC;



class NetGameLayer : public CCLayer
{
public:
	NetGameLayer(void);
	~NetGameLayer(void);
	virtual bool init();
	bool initBackGround();

	void CreateNetWork();//´´½¨ÍøÂç
	CREATE_FUNC(NetGameLayer);
private: 
	 ODSocket cSocket;
};