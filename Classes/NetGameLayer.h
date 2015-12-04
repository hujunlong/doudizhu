#pragma once
#include "cocos2d.h"
#include "MySocket.h"
#include "protocol.pb.h"
#include "MyTextFieldTTF.h"

#include <string>
#include <fstream>

USING_NS_CC;
using namespace std;


struct LoginInfo
{
	string name;
	string pwd;
};

class NetGameLayer : public CCLayer
{
public:
	NetGameLayer(void);
	~NetGameLayer(void);
	virtual bool init();
	bool initBackGround();

	void CreateNetWork();//创建网络
	bool GetLoginInfo();//读取玩家的登陆 账号 密码

	CREATE_FUNC(NetGameLayer);

private: 
	 ODSocket cSocket;
	 LoginInfo login_info;//登陆相关消息
	 MyTextFieldTTF *register_text;
};