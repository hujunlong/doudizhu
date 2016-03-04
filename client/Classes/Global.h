#pragma once
#include <vector>
#include "MySocket.h"

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
	PLANE_PK = 1,			//飞机
	BOMB_PK,				//炸弹
	SINGLE_PK,				//单牌
	DOUBLE_PK,				//双牌
	THREE_NO_ADD_PK,		//3不带
	THREE_ONE_PK,			//3带1-
	THREE_DOUBLE_PK,		//3带2-
	SINGLE_SHUNZHI_PK,		//单连
	DOUBLE_SHUNZHI_PK,      //双连
	THREE_SHUNZHI_PK,       //三连
	PLANE_ADD_WINGS_SINGLE_PK,//飞机带翅膀
	PLANE_ADD_WINGS_DOUBLE_PK,//飞机带翅膀
	BOMB_TWO_PK,			//四个带2张单牌
	BOMB_DOUBLE_TWO_PK,		//四个带2对
	ERROR_PK				//错误的牌型
};


enum PLAYER_HANDLE_TYPE //玩家种类:0为玩家用于的牌，1为电脑有，2为显示的三张牌,3为玩家要出的牌，4为电脑1要出的牌，5为电脑2要出的牌
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

struct PkStruct //牌 类型 大小
{
	int pk_type;
	int pk_num;
};

enum TypeId{
	GetTypeID = 0,
	LoginInfoID = 1,
	RegisterInfoID = 2,
	RequestID = 3,
	ResultChatMsgID = 4,
	ResultInfoID = 5,
};

enum Result{
	REGISTERSUCCESS = 0, //注册成功
	REGISTERERROR   = 1, //注册错误
	LOGINERROR      = 2, //登陆错误
	PASSWDERROR     = 3, //密码错误
	SAMENICK        = 4, //注册名字相同
	LOGINSUCCESS    = 5, //登陆成功
};