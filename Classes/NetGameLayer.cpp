#include "NetGameLayer.h"
 
NetGameLayer::NetGameLayer(){
	
}

NetGameLayer::~NetGameLayer(){
	
}

bool NetGameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	
	initBackGround();
	 return true; 
}
 

bool NetGameLayer::initBackGround(){
	bool isRet = false;
	CCSprite *bk = CCSprite::create("bk.png");
	this->addChild(bk,0);
	bk->setAnchorPoint(ccp(0,0));
	isRet = true;
	return isRet;
}

void NetGameLayer::CreateNetWork(){
	cSocket.Init();
	cSocket.Create(AF_INET,SOCK_STREAM,0);
	cSocket.Connect("127.0.0.1",7880);
}