#include "MyTextFieldTTF.h"

CCScene* MyTextFieldTTF::scene()
{
	CCScene* scene = CCScene::create();
	MyTextFieldTTF* layer = MyTextFieldTTF::create();
	scene->addChild(layer);
	return scene;
}


bool MyTextFieldTTF::init()
{
	CCLayer::init();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	text_name = CCTextFieldTTF::textFieldWithPlaceHolder("<click here for name>",
		"Arial", 30);
	//设置文本框的位置
	text_name->setPosition(ccp(winSize.width / 2 - 50, winSize.height / 2+20));
	//添加文本框到层上
	addChild(text_name);

	text_pwd = CCTextFieldTTF::textFieldWithPlaceHolder("<click here for pwd>",
		"Arial", 30);
	//设置文本框的位置
	text_pwd->setPosition(ccp(winSize.width / 2-50, winSize.height / 2-20));
	//添加文本框到层上
	addChild(text_pwd);

	//当触摸到控件的时候弹出软键盘
	setTouchMode(kCCTouchesOneByOne);
	setTouchEnabled(true);

	return true;
}
