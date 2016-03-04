#pragma once
#include "cocos2d.h"
#include "Rule.h"
#include "Player.h"
#include "Poker.h"

USING_NS_CC;

//单机游戏
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
	void menuLost(CCObject* sender);//失败
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
	void ClearOutPks(bool is_player_pre =false);//清除所有出牌
	bool NpcOutPks(Player* m_npc,Player* m_npcOut);//NPC出牌
	bool NpcFollowPks(Player* m_npc,Player* m_npcOut);//NPC跟牌
	void SplitPks(int type,Player* m_npc);//传入npc编号 1：npc1 2:npc2
	bool GameOver();//结果检查
	void OutPk(float delta);

private: 
	 CCArray* m_arrPokers;//所以的牌
	 CC_SYNTHESIZE(CCArray*,m_arrayPlayerOut,ArrayPlayerOut);//玩家选中的牌
	 CC_SYNTHESIZE(Player*,m_playerOut,PlayerOut);//玩家出的牌
	 CC_SYNTHESIZE(Player*,m_player,Player);//玩家

	 Player* m_npcOne;//电脑1
	 Player* m_npcTwo;//电脑2
	 Player* m_Three;//显示三张剩余牌
	 Player* m_npcOneOut;//电脑1出的牌
	 Player* m_npcTwoOut;//电脑2出的牌

	 Rule rule;//牌规则
	 StructCallPk m_call;//分数叫牌 (0 1 2:玩家 电脑1 电脑2)
	 int m_callTime;//轮流叫地主
	 int m_outPk;//该某人出牌

	 PkHandType old_handle_pks;//上一把出牌
	 Menu* m_menu;//叫地主菜单
	 Menu* m_handle_menu;//出牌菜单
	 Menu* m_success_menu;//成功菜单
	 Menu* m_lost_menu;//输了菜单
	 CCLabelTTF* m_lableDiZhu;//地主标签
	 CCLabelTTF* showScore;//显示叫牌分数
	 int m_state;//状态 0 发牌 1选地主 2打牌
	 int m_sendPk_num;//发牌 发了多少张了
	 bool is_split[3];
};