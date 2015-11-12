#pragma once
#include "cocos2d.h"
#include "Rule.h"
#include "Player.h"
#include "Poker.h"

USING_NS_CC;

class GameLayer : public CCLayer
{
public:

	GameLayer(void);
	~GameLayer(void);
	virtual bool init();
	CREATE_FUNC(GameLayer);

	bool initBackGround();
	bool createPokers();
	Poker *selectPoker(int pokerType,int num);
	bool initPlayer();
	bool initButton();
	void ShowScore(CCPoint pt,int score);//显示分数 返回字体宽度
	void menuCallBackOneScore(CCObject* sender);
	void menuCallBackTwoScore(CCObject* sender);
	void menuCallBackThreeScore(CCObject* sender);
	void menuCallDiZhu(CCObject* sender);//不叫地主
	void menuNotHandle(CCObject* sender);//不出
	void menuHandle(CCObject* sender);//出牌
	void menuSuccess(CCObject* sender);//胜利
	void reStart();//重新开始
	bool IsOutPkFinish();//是否出完牌
	void sendPk();//发牌
	void MovePk(Player* play,Poker* pk);//发牌特效
	void func(CCNode* pSender, void* pData);
	void update(float delta);
	void playerPkCanClick();
	void Call(float dt);//叫地主
	void NpcCall(Player* npc,int index);//电脑叫牌
	void GiveDiZhuThreePk();//给地主三张牌
private: 
	 CCArray* m_arrPokers;//所以的牌
	 CC_SYNTHESIZE(CCArray*,m_arrayPlayerOut,ArrayPlayerOut);//玩家要出的牌
	 CC_SYNTHESIZE(Player*,m_playerOut,PlayerOut);//玩家出的牌
	 CC_SYNTHESIZE(Player*,m_player,Player);//玩家
	 Player* m_npcOne;//电脑1
	 Player* m_npcTwo;//电脑2
	 Player* m_Three;//显示三张剩余牌
	 Player* m_npcOneOut;//电脑1出的牌
	 Player* m_npcTwoOut;//电脑2出的牌

	 Rule rule;//牌规则
	 //bool m_isCall[3];//分数叫牌
	 StructCallPk m_call;//分数叫牌 (0 1 2:玩家 电脑1 电脑2)
	 int m_callTime;//轮流叫地主
	 int m_outPk;//该某人出牌
	 CCArray* m_arrFollowPk;//要跟的牌

	 Menu* m_menu;//叫地主菜单
	 Menu* m_handle_menu;//出牌菜单
	 Menu* m_success_menu;//成功菜单
	 Menu* m_lost_menu;//输了菜单

	 CCLabelTTF* m_lableDiZhu;
	 int m_state;
	 int m_sendPk_num;
};