#include "GameLayer.h"
 
GameLayer::GameLayer():m_callTime(0),m_outPk(0){
	m_arrPokers = CCArray::create();
	m_arrPokers->retain();

	m_player = new Player();
	m_npcOne = new Player();
	m_npcTwo = new Player();
	m_Three = new Player();
	m_npcOneOut = new Player();
	m_npcTwoOut = new Player();
	for(int i=0; i<3; ++i)
		m_isCall[i] = false; 

	strings = Dictionary::createWithContentsOfFile("strings.xml");
	m_state = 0;
	m_sendPk_num = 0;
}

GameLayer::~GameLayer(){
	 CC_SAFE_RELEASE(m_arrPokers);
	 CC_SAFE_RELEASE(m_player);
	 CC_SAFE_RELEASE(m_npcOne);
	 CC_SAFE_RELEASE(m_npcTwo);
	 CC_SAFE_RELEASE(m_Three);
	 CC_SAFE_RELEASE(m_npcOneOut);
	 CC_SAFE_RELEASE(m_npcTwoOut);
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
		CC_BREAK_IF(!initButton());
	} while (0);

	scheduleUpdate();
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


void GameLayer::playerPkCanClick(){
	//主玩家可以点击自己的牌
	CCObject* object;
	CCARRAY_FOREACH(m_player->getArrPK(),object){
		Poker* pk = (Poker *)object;
		pk->setClick(true);
	}
}
bool GameLayer::initPlayer(){
	Size size = Director::getInstance()->getVisibleSize();

	//设置主玩家的位置
	m_player->setStartLocation(ccp(size.width/2,size.height/6));
	m_player->setHandleType(PLAYER_ARRAY_PK);

	//设置npc1
	m_npcOne->setStartLocation(ccp(65,504));
	m_npcOne->setHandleType(COMPUTER_ARRAY_PK);

	//设置npc2
	m_npcTwo->setStartLocation(ccp(735,504));
	m_npcTwo->setHandleType(COMPUTER_ARRAY_PK);

	//设置3张牌
	m_Three->setStartLocation(ccp(size.width/2,504));
	m_Three->setHandleType(THREE_PK);

	//主玩家出牌位置
	m_player->setStartLocation(ccp(size.width/2,size.height/6+106));
	m_player->setHandleType(PLAYER_HAND_PK);

	//NPC1 出牌位置
	m_npcOneOut->setStartLocation(ccp(146,size.height/2+20));
	m_npcOneOut->setHandleType(COMPUTER_1_HANDLE_PK);

	//NPC2 出牌位置
	m_npcTwoOut->setStartLocation(ccp(654,size.height/2+20));
	m_npcTwoOut->setHandleType(COMPUTER_2_HANDLE_PK);
	return true;
}
 

void GameLayer::ShowScore(CCPoint pt,int score){
	const char *score_str;
	if (score == score_zero)
		score_str  = ((String*)strings->objectForKey("bujiao"))->getCString();
	if (score == score_one)
		score_str  = ((String*)strings->objectForKey("yifen"))->getCString();
	if (score == score_two)
		score_str  = ((String*)strings->objectForKey("erfen"))->getCString();
	if (score == score_three)
		score_str  = ((String*)strings->objectForKey("sanfen"))->getCString();

	CCLabelTTF* showScore = CCLabelTTF::create(score_str,"Helvetica-BoldOblique",20);
	showScore->setPosition(ccp(pt.x,pt.y));
	showScore->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(showScore,1,PlayerScore);
}

 
void GameLayer::menuCallBackOneScore(CCObject* sender){
	m_player->setCallScore(score_one);
	m_player->setCall(true);
	//m_isCall[0] = true;
	//++m_callTime;
	 
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_one);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(true);
	 
}
 


void GameLayer::menuCallBackTwoScore(CCObject* sender){
	m_player->setCallScore(score_two);
	m_player->setCall(true);
	//m_isCall[1] = true;
	//++m_callTime;
 
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_two);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(true);
 
}


void GameLayer::menuCallBackThreeScore(CCObject* sender){
	m_player->setCallScore(score_three);
	m_player->setCall(true);
	m_npcOne->setCall(true);
	m_npcTwo->setCall(true);
 
	//m_isCall[2] = true;
	//++m_callTime;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_three);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(true);
}

void GameLayer::menuCallDiZhu(CCObject* sender){
	m_player->setCallScore(score_zero);
	m_player->setCall(true);
	//++m_callTime;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),0);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(true);
}

void GameLayer::menuNotHandle(CCObject* sender){
	++m_outPk;
	while (m_arrayPlayerOut->count() > 0){
		Poker* pk = (Poker*)m_arrayPlayerOut->objectAtIndex(0);
		if (pk->getSelect())
			pk->selectPkDown();
	}

	m_player->updatePkPosion();
	m_player->setIsOutPk(false);
}

bool GameLayer::IsOutPkFinish(){
	return false;
}

void GameLayer::menuHandle(CCObject* sender){
	CCObject *object;
	CCARRAY_FOREACH(m_playerOut->getArrPK(),object){ //TODO m_playerOut->getArrPK() ???
		Poker* pk = (Poker*)object;
		pk->setVisible(false);
	}

	m_playerOut->getArrPK()->removeAllObjects();

	m_playerOut->getArrPK()->addObjectsFromArray(m_arrayPlayerOut);
	m_arrFollowPk->removeAllObjects();

	m_arrFollowPk->addObjectsFromArray(m_arrayPlayerOut);
	m_arrayPlayerOut->removeAllObjects();

	m_playerOut->updatePkPosion();
	CCARRAY_FOREACH(m_playerOut->getArrPK(),object){
		Poker* pk = (Poker*)object;
		pk->setClick(false);
		m_player->getArrPK()->removeObject(pk);
	}

	m_player->updatePkPosion();
	if (IsOutPkFinish()){

	}else{
		++m_outPk;
	}
	
}

void GameLayer::reStart(){

}
void GameLayer::menuSuccess(CCObject* sender){
	reStart();
}

bool GameLayer::initButton(){
	Size size = Director::sharedDirector()->getVisibleSize();
	const char *tip;

	//1分
	tip = ((String *)strings->objectForKey("yifen"))->getCString();
	MenuItemFont *menu_one = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuCallBackOneScore));
	Size fontWidth = menu_one->getContentSize();
	int x =  size.width / 2 - (fontWidth.width * 4 + menu_score_distance * 3) / 2;
	int y =  m_player->getStartLocation().y + menu_score_top;
	menu_one->setAnchorPoint(ccp(0,0.5));
	menu_one->setPosition(ccp(x,y));

	//二分
	tip = ((String *)strings->objectForKey("erfen"))->getCString();
	MenuItemFont* menu_two = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuCallBackTwoScore));
	menu_two->setAnchorPoint(ccp(0,0.5));
	menu_two->setPosition(ccp(x+fontWidth.width*1+menu_score_distance,y));


	//三分
	tip = ((String *)strings->objectForKey("sanfen"))->getCString();
	MenuItemFont* menu_three = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuCallBackThreeScore));
	menu_three->setAnchorPoint(ccp(0,0.5));
	menu_three->setPosition(ccp(x+fontWidth.width*2+menu_score_distance*2,y));

	//不叫
	tip = ((String *)strings->objectForKey("bujiao"))->getCString();
	MenuItemFont* menu_buJiao = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuCallDiZhu));
	menu_buJiao->setAnchorPoint(ccp(0,0.5));
	menu_buJiao->setPosition(ccp(x+fontWidth.width*3+menu_score_distance*3,y));

	//叫地主菜单
	m_menu = Menu::create();
	m_menu->addChild(menu_one,1,score_one);
	m_menu->addChild(menu_two,1,score_two);
	m_menu->addChild(menu_three,1,score_three);
	m_menu->addChild(menu_buJiao,1,score_zero);
	this->addChild(m_menu);
	m_menu->setPosition(CCPointZero);
	m_menu->setVisible(false);

	//player不出
	x = size.width/2 - (fontWidth.width*2 + menu_score_distance*3)/2;
	tip = ((String *)strings->objectForKey("buchu"))->getCString();
	MenuItemFont* buChu = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuNotHandle));
	buChu->setAnchorPoint(ccp(0,0.5));
	buChu->setPosition(ccp(x,y));

	//出牌
	tip = ((String*)strings->objectForKey("chupai"))->getCString();
	MenuItemFont* chuPai = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuNotHandle));
	chuPai->setAnchorPoint(ccp(0,0.5));
	chuPai->setPosition(ccp(x+fontWidth.width+20,y));

	//出牌菜单
	m_handle_menu = Menu::create();
	m_handle_menu->addChild(buChu,2,0);
	m_handle_menu->addChild(chuPai,2,1);
	m_handle_menu->setPosition(CCPointZero);
	this->addChild(m_handle_menu,1000);
	m_handle_menu->setVisible(false);


	//胜利
	tip = ((String *)strings->objectForKey("youwin"))->getCString();
	MenuItemFont* menu_scucess = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuSuccess));
	menu_scucess->setAnchorPoint(ccp(0.5,0.5));
	menu_scucess->setPosition(ccp(size.width/2,size.height/2));

	m_success_menu = Menu::create();
	m_success_menu->addChild(menu_scucess);
	m_success_menu->setPosition(CCPointZero);
	this->addChild(m_success_menu);
	m_success_menu->setVisible(true);

	//你输了的按钮
	tip = ((String *)strings->objectForKey("youlost"))->getCString();
	MenuItemFont* lost = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuSuccess));
	lost->setAnchorPoint(ccp(0.5,0.5));
	lost->setPosition(ccp(size.width/2,size.height/2));

	m_lost_menu = Menu::create();
	m_lost_menu->addChild(lost);
	m_lost_menu->setPosition(CCPointZero);
	this->addChild(m_lost_menu);
	m_lost_menu->setVisible(true);

 
	//npctwo标签
	tip = ((String *)strings->objectForKey("buchu"))->getCString();
	CCLabelTTF* npcTwoNoHandle = CCLabelTTF::create(tip,"Helvetica-BoldOblique",20);
	npcTwoNoHandle->setPosition(m_npcTwoOut->getStartLocation());
	this->addChild(npcTwoNoHandle,1,NpcTwoNotPlay);
	npcTwoNoHandle->setVisible(false);

	//npcone标签
	CCLabelTTF* npcOneNoHandle = CCLabelTTF::create(tip,"Helvetica-BoldOblique",20);
	npcOneNoHandle->setPosition(m_npcOneOut->getStartLocation());
	this->addChild(npcOneNoHandle,1,NpcOneNotPlay);
	npcOneNoHandle->setVisible(false);

	//地主标签
	tip = ((String *)strings->objectForKey("dizhu"))->getCString();
	m_lableDiZhu = CCLabelTTF::create(tip,"Helvetica-BoldOblique",20);
	m_lableDiZhu->setPosition(ccp(100,100));
	this->addChild(m_lableDiZhu,1);
	m_lableDiZhu->setVisible(false);
	
	return true;
}

void GameLayer::sendPk(){
	Poker* pk;

	if(m_sendPk_num < 51){
		pk = (Poker*)m_arrPokers->objectAtIndex(m_sendPk_num);
		if (m_sendPk_num%3 == 0){//给玩家发牌
			MovePk(m_player,pk);
		}

		if (m_sendPk_num%3 == 1){//给玩家发牌
			MovePk(m_npcOne,pk);
		}

		if (m_sendPk_num%3 == 2){//给玩家发牌
			MovePk(m_npcTwo,pk);
		}
		++m_sendPk_num;

	}else if (m_sendPk_num>50 && m_sendPk_num<54){
		pk = (Poker*)m_arrPokers->objectAtIndex(m_sendPk_num);
		pk->showBack();
		MovePk(m_Three,pk);
		++m_sendPk_num;
	}

	if (m_sendPk_num == 53)
	{
		playerPkCanClick();
	}
}

void GameLayer::func(CCNode* pSender, void* pData){
	Player* play = (Player*)pData;
	play->updatePkPosion();
}

void GameLayer::MovePk(Player* play,Poker* pk){
	CCMoveTo* move;
	CCCallFuncND* func;
	float time = 0.05;
	play->getArrPK()->addObject(pk);
	move=CCMoveTo::create(time,play->getStartLocation());
	func = CCCallFuncND::create(this,callfuncND_selector(GameLayer::func),play);
	CCSequence* sequence = CCSequence::create(move,func,NULL);
	pk->runAction(sequence);
}

void GameLayer::update(float delta){
	switch (m_state)
	{
	case 0:
		sendPk();
	case 1:
	case 2:
	case 3:

	default:
		break;
	}
}