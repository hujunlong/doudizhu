#include "GameLayer.h"


GameLayer::GameLayer(){
	m_arrPokers = CCArray::create();
	m_arrPokers->retain();

	m_player = new Player();
	m_npcOne = new Player();
	m_npcTwo = new Player();
	m_Three = new Player();
	 
}

GameLayer::~GameLayer(){
	 CC_SAFE_RELEASE(m_arrPokers);
	 CC_SAFE_RELEASE(m_player);
	 CC_SAFE_RELEASE(m_npcOne);
	 CC_SAFE_RELEASE(m_npcTwo);
	 CC_SAFE_RELEASE(m_Three);
}

bool GameLayer::init(){
	if(!Layer::init()){
		return false;
	}

	srand((unsigned)time(NULL));
	do 
	{
		CC_BREAK_IF(!initBackGround());
		CC_BREAK_IF(!createPokers());
		CC_BREAK_IF(!initPlayer());
	} while (0);

	 return true; 
}
 

bool GameLayer::initBackGround(){
	bool isRet = false;
	CCSprite *bk = CCSprite::create("bk.png");
	this->addChild(bk,0);
	bk->setAnchorPoint(ccp(0,0));
	isRet = true;
	return isRet;
}

Poker *GameLayer::selectPoker(int pokerType,int num){
	Poker* pk;
	if (pokerType != Joker)
		pk = Poker::create("poker.png",CCRect(num*pkWidth,pokerType*pkHeight,pkWidth,pkHeight));
	else
		pk = Poker::create("poker.png",CCRect((num-BlackJoker)*pkWidth,pokerType*pkHeight,pkWidth,pkHeight));
	pk->setPkType(pokerType);
	pk->setPkNum(num);
	pk->setGameMain(this);
	return pk;
}

bool GameLayer::createPokers(){
	bool isRet = false;
	Size size = Director::sharedDirector()->getVisibleSize();
	Poker* pk;

	rule.InitPks();
	rule.Shuffle();
	for (int i=0;i<54;++i)
	{
		pk = selectPoker(rule.pks[i].pk_type,rule.pks[i].pk_num);
		pk->setPosition(ccp(size.width/2,size.height/2));
		pk->showBack();
		this->addChild(pk);
		this->m_arrPokers->addObject(pk);
	}
	isRet = true;
	return isRet;
}

bool GameLayer::initPlayer(){
	Size size = Director::getInstance()->getVisibleSize();

	//设置主玩家的位置
	m_player->setStartLocation(ccp(size.width/2,size.height/6));
	m_player->setHandleType(PLAYER_ARRAY_PK);

	//设置npc1
	m_player->setStartLocation(ccp(65,504));
	m_player->setHandleType(COMPUTER_ARRAY_PK);

	//设置npc2
	m_player->setStartLocation(ccp(735,504));
	m_player->setHandleType(COMPUTER_ARRAY_PK);

	//设置3张牌
	m_player->setStartLocation(ccp(size.width/2,504));
	m_player->setHandleType(THREE_PK);

	//主玩家出牌位置
	m_player->setStartLocation(ccp(size.width/2,size.height/6+106));
	m_player->setHandleType(PLAYER_HAND_PK);

	//NPC1 出牌位置
	m_player->setStartLocation(ccp(146,size.height/2+20));
	m_player->setHandleType(PLAYER_HAND_PK);

	//NPC2 出牌位置
	m_player->setStartLocation(ccp(654,size.height/2+20));
	m_player->setHandleType(PLAYER_HAND_PK);
	return true;
}
 
bool GameLayer::initButton(){
	Size size = Director::sharedDirector()->getVisibleSize();
	Dictionary* strings = Dictionary::createWithContentsOfFile("strings.xml");
}