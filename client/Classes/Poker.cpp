#include "Poker.h"
#include "GameLayer.h"

Poker::Poker():m_isSelect(false),m_isClick(false){

}

Poker::~Poker(){

}

Poker* Poker::create(const char *poker_name,const CCRect& rect){
	Poker* pk = new Poker();
	if (pk && pk->initWithFile(poker_name,rect)){
		pk->autorelease();
		return pk;
	}
	CC_SAFE_DELETE(pk);
	return pk;
}

void Poker::onEnter(){
	CCSprite::onEnter();
 
    touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Poker::onTouchBegan,this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Poker::onTouchMoved,this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Poker::onTouchEnded,this);
	touchListener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);
}


void Poker::onExit(){
	_eventDispatcher->removeEventListenersForTarget(this);
	CCSprite::onExit();
}


bool Poker::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCSize size = getContentSize();
	CCRect rect(-size.width/2,-size.height/2,size.width,size.height);
	CCPoint ptouch = convertTouchToNodeSpaceAR(pTouch);
	if (rect.containsPoint(ptouch) && m_isClick){
		if (!m_isSelect)
			selectPkTop();
		else
			selectPkDown();
		return true;
	}
	return false;
}


void Poker::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void Poker::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

void Poker::onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}

void Poker::showFront(){
	if (m_PkType != Joker){
		this->setTextureRect(CCRect(this->m_PkNum*pkWidth,this->m_PkType*pkHeight,pkWidth,pkHeight));
	}else{
		this->setTextureRect(CCRect((this->m_PkNum-BlackJoker)*pkWidth,this->m_PkType*pkHeight,pkWidth,pkHeight));
	}
}

void Poker::showBack(){
	this->setTextureRect(CCRect(PkBackNum*pkWidth,PKBack*pkHeight,pkWidth,pkHeight));
}

Poker* Poker::copy(){
	Poker* pk;
	if (m_PkType != Joker)
		pk = Poker::create("poker.png",CCRect(this->m_PkNum*pkWidth,this->m_PkType*pkHeight,pkWidth,pkHeight));
	else
		pk = Poker::create("poker.png",CCRect((this->m_PkNum-BlackJoker)*pkWidth,this->m_PkType*pkHeight,pkWidth,pkHeight));
	pk->m_isClick = this->m_isClick;
	pk->m_isSelect = this->m_isSelect;
	pk->setPkType(this->getPkType());
	pk->setPkNum(this->getPkNum());
	pk->m_gameMain = this->m_gameMain;
	return pk;
}

void Poker::setTouchPriority(int num){
	_eventDispatcher->setPriority(touchListener,num);
}

void Poker::selectPkTop(){
	this->m_isSelect = true;
	this->setPosition((ccp(getPositionX(),getPositionY() + pkTopHeight)));
	m_gameMain->getArrayPlayerOut()->addObject(this);
	//m_gameMain->PlayerOutPaiXu(m_gameMain->getArrPlayerOut());
}

void Poker::selectPkDown(){
	m_isSelect = false;
	this->setPosition(ccp(getPositionX(),getPositionY() - pkTopHeight));
	m_gameMain->getArrayPlayerOut()->removeObject(this);
}