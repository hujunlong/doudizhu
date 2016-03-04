#include "ChoosePlayTypeLayer.h"
 
 
ChoosePlayTypeLayer::ChoosePlayTypeLayer(){
	
}

ChoosePlayTypeLayer::~ChoosePlayTypeLayer(){
	 
}

bool ChoosePlayTypeLayer::init(){

	Size visibleSize = Director::getInstance()->getVisibleSize();  
	CCSprite *bk = CCSprite::create("bk.png");
	this->addChild(bk,0);
	bk->setAnchorPoint(ccp(0,0));

	const char *tip;
	Dictionary* strings = Dictionary::createWithContentsOfFile("strings.xml");

	tip = ((String *)strings->objectForKey("danji"))->getCString();
	MenuItemFont* alone_menu = MenuItemFont::create(tip,this,menu_selector(ChoosePlayTypeLayer::menuAloneHandle));
	alone_menu->setPosition(Point(visibleSize.width / 2 - 20, visibleSize.height/2 + 20));

 
	tip = ((String*)strings->objectForKey("wangluo"))->getCString();
	MenuItemFont* net_menu = MenuItemFont::create(tip,this,menu_selector(ChoosePlayTypeLayer::menuNetHandle));
	net_menu->setPosition(Point(visibleSize.width / 2 - 20, visibleSize.height/2 - 20));

	m_handle_menu = Menu::create();
	m_handle_menu->addChild(alone_menu,2,0);
	m_handle_menu->addChild(net_menu,2,1);
	m_handle_menu->setPosition(CCPointZero);
	this->addChild(m_handle_menu,20);



	 return true; 
}
 

void ChoosePlayTypeLayer::menuAloneHandle(CCObject* sender){

	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);

}

void ChoosePlayTypeLayer::menuNetHandle(CCObject* sender){
	auto scene = Scene::create();
	auto layer = NetGameLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}