#include "GameLayer.h"
 
GameLayer::GameLayer():m_callTime(0),m_outPk(0){
	m_arrPokers = CCArray::create();
	m_arrPokers->retain();

	m_player = new Player();
	m_npcOne = new Player();
	m_npcTwo = new Player();
	m_Three = new Player();
	m_playerOut = new Player();
	m_npcOneOut = new Player();
	m_npcTwoOut = new Player();

	m_arrayPlayerOut = CCArray::create();
	m_arrayPlayerOut->retain();

	m_call.People = 0;
	m_call.CallScore = 0;

	m_state = 0;
	m_sendPk_num = 0;

	srand ((unsigned)time(nullptr));
	m_callTime = rand()%3;

	for (int i=0;i<3;i++){
		is_split[i] = false;
	}
}

GameLayer::~GameLayer(){
	 CC_SAFE_RELEASE(m_arrPokers);
	 CC_SAFE_RELEASE(m_player);
	 CC_SAFE_RELEASE(m_npcOne);
	 CC_SAFE_RELEASE(m_npcTwo);
	 CC_SAFE_RELEASE(m_Three);
	 CC_SAFE_RELEASE(m_playerOut);
	 CC_SAFE_RELEASE(m_npcOneOut);
	 CC_SAFE_RELEASE(m_npcTwoOut);
}

bool GameLayer::init(){
	if(!Layer::init()){
		return false;
	}
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
		//pk->showBack();
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
	m_npcOne->setStartLocation(ccp(735,504));
	m_npcOne->setHandleType(COMPUTER_ARRAY_PK);

	//设置npc2
	m_npcTwo->setStartLocation(ccp(65,504));
	m_npcTwo->setHandleType(COMPUTER_ARRAY_PK);

	//设置3张牌
	m_Three->setStartLocation(ccp(size.width/2,504));
	m_Three->setHandleType(THREE_PK);

	//主玩家出牌位置
	m_playerOut->setStartLocation(ccp(size.width/2,size.height/6+106));
	m_playerOut->setHandleType(PLAYER_HAND_PK);

	//NPC1 出牌位置
	m_npcOneOut->setStartLocation(ccp(354,size.height/2+20));
	m_npcOneOut->setHandleType(COMPUTER_1_HANDLE_PK);

	//NPC2 出牌位置
	m_npcTwoOut->setStartLocation(ccp(146,size.height/2+20));
	m_npcTwoOut->setHandleType(COMPUTER_2_HANDLE_PK);
	return true;
}
 

void GameLayer::ShowScore(CCPoint pt,int score){
	const char *score_str;
	Dictionary* strings = Dictionary::createWithContentsOfFile("strings.xml");
	if (score == score_zero)
		score_str  = ((String*)strings->objectForKey("bujiao"))->getCString();
	if (score == score_one)
		score_str  = ((String*)strings->objectForKey("yifen"))->getCString();
	if (score == score_two)
		score_str  = ((String*)strings->objectForKey("erfen"))->getCString();
	if (score == score_three)
		score_str  = ((String*)strings->objectForKey("sanfen"))->getCString();

	showScore->setString(score_str);
	showScore->setPosition(ccp(pt.x,pt.y));
	showScore->setAnchorPoint(ccp(0.5,0.5));
}

 
void GameLayer::menuCallBackOneScore(CCObject* sender){
	if (m_call.CallScore >= 1){
		++m_callTime;
		m_player->setCall(true);
		return;
	}
	m_player->setCallScore(score_one);
	m_player->setCall(true);
	//m_isCall[0] = true;
	m_call.People = 0;
	m_call.CallScore = 1;
	++m_callTime;
	 
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_one);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(false); 
}
 


void GameLayer::menuCallBackTwoScore(CCObject* sender){
	if (m_call.CallScore >= 2){
		++m_callTime;
		m_player->setCall(true);
		return;
	}

	m_player->setCallScore(score_two);
	m_player->setCall(true);
	m_call.People = 0;
	m_call.CallScore = 2;
	++m_callTime;
 
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_two);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(false);
 
}


void GameLayer::menuCallBackThreeScore(CCObject* sender){
	if (m_call.CallScore >= 3){
		++m_callTime;
		m_player->setCall(true);
		return;
	}

	m_player->setCallScore(score_three);
	m_player->setCall(true);
	m_npcOne->setCall(true);
	m_npcTwo->setCall(true);
	m_call.People = 0;
	m_call.CallScore = 3;
	++m_callTime;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),score_three);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(false);
}

void GameLayer::menuCallDiZhu(CCObject* sender){
	m_player->setCallScore(score_zero);
	m_player->setCall(true);
	++m_callTime;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	ShowScore(CCPoint(size.width/2,m_player->getStartLocation().y+menu_score_top),0);
	CCMenuItemFont* font = (CCMenuItemFont*) sender;
	font->getParent()->setVisible(false);
}

void GameLayer::menuNotHandle(CCObject* sender){
	//++m_outPk;
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
		//++m_outPk;
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
	Dictionary* strings = Dictionary::createWithContentsOfFile("strings.xml");
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
	MenuItemFont* chuPai = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuHandle));
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
	m_success_menu->setVisible(false);

	//你输了的按钮
	tip = ((String *)strings->objectForKey("youlost"))->getCString();
	MenuItemFont* lost = MenuItemFont::create(tip,this,menu_selector(GameLayer::menuSuccess));
	lost->setAnchorPoint(ccp(0.5,0.5));
	lost->setPosition(ccp(size.width/2,size.height/2));

	m_lost_menu = Menu::create();
	m_lost_menu->addChild(lost);
	m_lost_menu->setPosition(CCPointZero);
	this->addChild(m_lost_menu);
	m_lost_menu->setVisible(false);

 
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
	

	showScore = CCLabelTTF::create("","Helvetica-BoldOblique",20);
	this->addChild(showScore,1,Score);

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
		//pk->showBack();
		MovePk(m_Three,pk);
		++m_sendPk_num;
	}

	if (m_sendPk_num > 53){
		++m_state;
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
		break;
	case 1:
		schedule(schedule_selector(GameLayer::Call),1);
		break;
	case 2:
		scheduleOnce(schedule_selector(GameLayer::OutPk),1);
	case 3:

	default:
		break;
	}
}

//npc Call
void GameLayer::NpcCall(Player* npc,int index){
	int num = rule.Call(index);
	if (m_call.CallScore < num){
		npc->setCallScore(num);
		m_call.CallScore = num;
		m_call.People = index;
	}	
	else
       npc->setCallScore(0);
	npc->setCall(true);
}

void GameLayer::GiveDiZhuThreePk(){
	CCPoint playerDiZhuLablePt = CCPointMake(100,100);
	CCPoint npcOneDiZhuLablePt = CCPointMake(735-100,504);
	CCPoint npcTwoDiZhuLablePt = CCPointMake(65+100,504);
	Size size = Director::sharedDirector()->getVisibleSize();
	CCObject* object;
	CCARRAY_FOREACH(m_Three->getArrPK(),object){
		Poker* pk = (Poker *)object;
		Poker* pkCopy = pk->copy();
		pkCopy->setPosition(ccp(size.width/2,size.height/2));
		
		this->addChild(pkCopy);

		if (m_call.People == 0){
			m_player->getArrPK()->addObject(pkCopy);	
		}else if (m_call.People == 1){
			m_npcOne->getArrPK()->addObject(pkCopy);
		}else{
			m_npcTwo->getArrPK()->addObject(pkCopy);
		}
		pk->showFront();
	}
	m_Three->updatePkPosion();
	//显示地主标签
	switch (m_call.People)
	{
	case 0:
		m_lableDiZhu->setPosition(playerDiZhuLablePt);
		m_player->updatePkPosion();
		m_player->setIsDiZhu(true);
		m_npcOne->setIsDiZhu(false);
		m_npcTwo->setIsDiZhu(false);
		m_outPk = 0;
		break;
	case 1:
		m_lableDiZhu->setPosition(npcOneDiZhuLablePt);
		m_npcOne->updatePkPosion();
		m_player->setIsDiZhu(false);
		m_npcOne->setIsDiZhu(true);
		m_npcTwo->setIsDiZhu(false);
		m_outPk = 1;
		break;
	case 2:
		m_lableDiZhu->setPosition(npcTwoDiZhuLablePt);
		m_npcTwo->updatePkPosion();
		m_player->setIsDiZhu(false);
		m_npcOne->setIsDiZhu(false);
		m_npcTwo->setIsDiZhu(true);
		m_outPk = 2;
		break;
	default:
		break;
	}
	
	rule.GiveDizhuPks(m_call.People);

	//主玩家牌可点击
	playerPkCanClick();
	this->unschedule(schedule_selector(GameLayer::Call));
	m_lableDiZhu->setVisible(true);

	//移除叫分
	showScore->setString("");

	++m_state;
}

//叫地主
void GameLayer::Call(float dt){
	m_callTime = m_callTime%3;
	if (m_call.CallScore >=3 )//已经叫地主了
	{
		GiveDiZhuThreePk();
		return;
	}

	if (!m_player->getCall() || !m_npcOne->getCall() || !m_npcTwo->getCall()){
		switch (m_callTime){
		case 0:
			m_menu->setVisible(true);

			if (m_call.CallScore < 1){
				CCMenuItemFont* itemFont = (CCMenuItemFont*)m_menu->getChildByTag(score_one);
				itemFont->setEnabled(true);
				return;
			}

			if (m_call.CallScore > 1 && m_call.CallScore <= 2){
				CCMenuItemFont* itemFont = (CCMenuItemFont*)m_menu->getChildByTag(score_two);
				itemFont->setEnabled(true);
				CCMenuItemFont* buff_itemFont = (CCMenuItemFont*)m_menu->getChildByTag(score_one);
				buff_itemFont->setEnabled(false);
				return;
			} 

			if (m_call.CallScore >= 2 && m_call.CallScore < 3){
				CCMenuItemFont* itemFont = (CCMenuItemFont*)m_menu->getChildByTag(score_three);
				itemFont->setEnabled(true);

				CCMenuItemFont* buff_itemFont1 = (CCMenuItemFont*)m_menu->getChildByTag(score_one);
				buff_itemFont1->setEnabled(false);

				CCMenuItemFont* buff_itemFont2 = (CCMenuItemFont*)m_menu->getChildByTag(score_two);
				buff_itemFont2->setEnabled(false);
			} 
			break;
		case 1:
			++m_callTime;
			NpcCall(m_npcOne,1);
			ShowScore(ccp(m_npcOne->getStartLocation().x-88,m_npcOne->getStartLocation().y),m_npcOne->getCallScore());
			break;
		case 2:
			++m_callTime;
			NpcCall(m_npcTwo,2);
			ShowScore(ccp(m_npcTwo->getStartLocation().x+88,m_npcTwo->getStartLocation().y),m_npcTwo->getCallScore());
			break;
		default:
			break;
		}
	}else{
		GiveDiZhuThreePk();
		return;
	} 
}

void GameLayer::OutPk(float delta){
	switch (m_outPk%3){
	case 0:
		m_handle_menu->setVisible(true);
		++m_outPk;
		break;
	case 1:
		if(!is_split[1]){
			is_split[1] = true;
			rule.AiSplitPks(1);
			SplitPks(1,m_npcOne);
		}
		
		ClearOutPks();
		NpcOutPks(1);
		++m_outPk;
		break;
	case 2:
		if(!is_split[2]){
			is_split[2] = true;
			rule.AiSplitPks(2);
			SplitPks(2,m_npcTwo);
		}
		ClearOutPks();
		NpcOutPks(2);
		++m_outPk;
		break;
	default:
		break;
	}
}

//清除所有出牌
void GameLayer::ClearOutPks(){
	CCObject *object;
	CCARRAY_FOREACH(m_playerOut->getArrPK(),object){//玩家出牌
		Poker *pk = (Poker*) object;
		pk->setVisible(false);
	}
	m_playerOut->getArrPK()->removeAllObjects();

	CCARRAY_FOREACH(m_npcOneOut->getArrPK(),object){//NPC1出牌
		Poker *pk = (Poker*) object;
		pk->setVisible(false);
	}
	m_npcOneOut->getArrPK()->removeAllObjects();

	CCARRAY_FOREACH(m_npcTwoOut->getArrPK(),object){//NPC2出牌
		Poker *pk = (Poker*) object;
		pk->setVisible(false);
	}
	m_npcTwoOut->getArrPK()->removeAllObjects();

	this->getChildByTag(NpcTwoNotPlay)->setVisible(false);
	this->getChildByTag(NpcOneNotPlay)->setVisible(false);
}

void GameLayer::NpcOutPks(int type){
	if (type == 1)
	{
		if (npc1_pk_type.size() <= 0 ){
			return;
		}

		for (int i=0;i<npc1_pk_type[0].vec.size();i++)
		{
			m_npcOneOut->getArrPK()->addObject(npc1_pk_type[0].vec[i]);
		}
		npc1_pk_type.erase(npc1_pk_type.begin());
		m_npcOneOut->updatePkPosion();
		m_npcOne->updatePkPosion();
	}
	else{
		if (npc2_pk_type.size() <= 0 ){
			return;
		}

		for (int i=0;i<npc2_pk_type[0].vec.size();i++)
		{
			m_npcTwoOut->getArrPK()->addObject(npc2_pk_type[0].vec[i]);
		}

		npc2_pk_type.erase(npc2_pk_type.begin());
		m_npcTwoOut->updatePkPosion();
		//m_npcTwo->updatePkPosion();
	}
}

void GameLayer::SplitPks(int type,Player* m_npc){

	Paixing npc_pk_type_;
	CCObject *object;
	
	CCLog("####################################");
	CCARRAY_FOREACH(m_npc->getArrPK(),object){
		Poker *pk = (Poker*) object;
		CCLog("type = %i value=%i",pk->getPkType(),pk->getPkNum() );
	}	
	CCLog("####################################");

	for (std::vector<PkHandleTypeStruct>::iterator iter= rule.vec_pk_hands.begin();iter != rule.vec_pk_hands.end();iter++){
		npc_pk_type_.type = iter->type;

		for (int i=0;i<iter->pk_structs.size();i++){
			CCARRAY_FOREACH(m_npc->getArrPK(),object){
				Poker *pk = (Poker*) object;
				if (pk->getPkType() == iter->pk_structs[i].pk_type && pk->getPkNum() == iter->pk_structs[i].pk_num){
					npc_pk_type_.vec.push_back(pk);
				}
			}	 
		}

		if (type == 1)
			npc1_pk_type.push_back(npc_pk_type_);
		else
			npc2_pk_type.push_back(npc_pk_type_);
	}

}