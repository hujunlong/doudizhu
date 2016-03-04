#pragma once
#include "cocos2d.h"
#include "protocol.pb.h"

#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UITextField.h" 

#include <string>
#include <fstream>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
using namespace ui;

class NetGameLayer : public CCLayer
{
public:
	NetGameLayer(void);
	~NetGameLayer(void);
	virtual bool init();
	void buttonEvent(cocos2d::Ref* pSender,cocos2d::ui::TouchEventType type);
	void CreateRegister();
	void CreateNetWork();//´´½¨ÍøÂç
	void Login();
	void LoginResult();
	CREATE_FUNC(NetGameLayer);
private: 
	 Button *commit;
	 ui::TextField* userName;
	 ui::TextField* passwd;

	 std::string str_user_name;
	 std::string str_passwd;
};