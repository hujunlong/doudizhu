#include "NetGameLayer.h"
 
NetGameLayer::NetGameLayer(){
	text_name = MyTextFieldTTF::create();
	text_pwd = MyTextFieldTTF::create();
}

NetGameLayer::~NetGameLayer(){
	
}

bool NetGameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	
	initBackGround();
	CreateNetWork();
	GetLoginInfo();
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

bool NetGameLayer::GetLoginInfo(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	const char *tip;
	Dictionary* strings = Dictionary::createWithContentsOfFile("strings.xml");

 	tip = ((String*)strings->objectForKey("user_name"))->getCString();
	text_name->CreateText(tip,ccp(winSize.width / 2 - 50, winSize.height / 2+20));
 	addChild(text_name,2);

	tip = ((String*)strings->objectForKey("pass_world"))->getCString();
	text_pwd->CreateText(tip,ccp(winSize.width / 2 - 50, winSize.height / 2-20));
	addChild(text_pwd,1);

	return false;
}