#pragma once
#include <vector>
using namespace std;

const int pkWidth = 71;//牌的宽度
const int pkHeight = 96;//牌的高度
const int pkDistance = 20;//牌与牌的间距
const int pkTopHeight = 10;//牌点击向上移动距离
const int menu_score_top = 88;//叫分向上位置高度
const int menu_score_distance = 20;//叫分间距
//花色
enum PKType
{
	Spade = 0,//黑桃
	Heart,//红桃
	Club,//梅花
	Dianmond,//方块
	Joker,//王
	PKBack = 4,//花色背面
};

enum PKNum {
	Num = 0,Four,Five,Six,Seven,Eight,Nine,Ten,Jack,Queen,King,Ace,Er,BlackJoker,RedJoker,
	PkBackNum = 2 //背面所在图片位置
};

enum Table {
	Score,
	NpcOneNotPlay,
	NpcTwoNotPlay
};

enum CARD_TYPE{
	SINGLE_CARD = 1,		//单牌-
	DOUBLE_CARD,			//对子-
	THREE_CARD,				//3不带-
	BOMB_CARD,				//炸弹
	THREE_ONE_CARD,			//3带1-
	THREE_TWO_CARD,			//3带2-
	BOMB_TWO_CARD,			//四个带2张单牌
	BOMB_DOUBLE_TWO_CARD,	//四个带2对
	CONNECT_CARD,			//连牌-
	COMPANY_CARD,			//连队-
	AIRCRAFT_CARD,			//飞机不带-
	AIRCRAFT_SINGLE_CARD,	//飞机带单牌-
	AIRCRAFT_DOULBE_CARD,	//飞机带对子-
	ERROR_CARD				//错误的牌型
};


enum PLAYER_HANDLE_TYPE ////玩家种类:0为玩家用于的牌，1为电脑有，2为显示的三张牌,3为玩家要出的牌，4为电脑1要出的牌，5为电脑2要出的牌
{
	PLAYER_ARRAY_PK = 0,
	COMPUTER_ARRAY_PK,
	THREE_PK,
	PLAYER_HAND_PK,
	COMPUTER_1_HANDLE_PK,
	COMPUTER_2_HANDLE_PK
};


enum SCORE{
	score_zero,
	score_one ,
	score_two,
	score_three
};
struct AIR_ADD_CARD_TYPE //飞机带牌
{
	std::vector<int> single_card;//单牌
	std::vector<int> double_card;//双牌
	std::vector<int> three_card;//三张
	std::vector<int> four_card;//四张
};

struct StructCallPk
{
	int People;
	int CallScore;
};