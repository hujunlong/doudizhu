#include "NetGameLayer.h"
#include "Global.h" 
#include <thread>
#include <boost/algorithm/string.hpp>  
#include <boost/asio.hpp>

extern ODSocket cSocket;
using namespace boost;
NetGameLayer::NetGameLayer(){

}

NetGameLayer::~NetGameLayer(){
	
}

bool NetGameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	 
	std::fstream file_;
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath();
	path += "/login.txt";
	file_.open(path,std::ios::in);
	//文件不存在
	if(!file_){ 
		CreateRegister();
	}else{
		file_>>str_user_name>>str_passwd;
		Login();
	}
	
	return true; 
}


void NetGameLayer::CreateRegister(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(NetGameLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(NetGameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto layout  = cocostudio::GUIReader::shareReader()->widgetFromJsonFile("login.json"); 
	layout->setAnchorPoint(ccp(0.5,0.5));
	layout->setPosition(Vec2(winSize.width/2,winSize.height/2) );
	this->addChild(layout); 

	commit = (ui::Button*)layout->getChildByName("commit");
	commit->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&NetGameLayer::buttonEvent));
	commit->setTag(1);

	userName = (ui::TextField*)layout->getChildByName("input_name");
	passwd = (ui::TextField*)layout->getChildByName("input_pwd");
}

void NetGameLayer::buttonEvent(cocos2d::Ref* pSender,ui::TouchEventType type){
	Button* button_ = (Button*)pSender;  
	unsigned int tag = button_->getTag();

	switch (type){
	case ui::TouchEventType::TOUCH_EVENT_BEGAN:
	{
		break;
	}

	case ui::TouchEventType::TOUCH_EVENT_ENDED:
	{
		//发送
		if (tag == 1){
			str_user_name = userName->getStringValue();
			str_passwd = passwd->getStringValue();
			trim(str_user_name);
			trim(str_passwd);
			
			std::thread thread_login(std::bind(&NetGameLayer::LoginResult,this));
			thread_login.join();
		}
		break;
	}									 
	default:
		break;
											  
	}
}

void NetGameLayer::Login(){
	protocol::S2SSystem_LoginInfo login_info;
	string order_str;
	login_info.set_pid(LoginInfoID);
	login_info.set_name(str_user_name);
	login_info.set_passworld(str_passwd);
	login_info.SerializeToString(&order_str);
	cSocket.Send(order_str.c_str(),order_str.length());
}

void NetGameLayer::LoginResult(){
	//后期使用boost asio异步通信
	Login();
	char recive_data[1024]={0};
	char recive_head[4]={0};
	int recive_data_len = cSocket.Recv((char*)&recive_data,sizeof(recive_data),0);
	if (recive_data_len == -1)//网络连接失败
	{
		return;
	}
	recive_data[recive_data_len] = '\0';
	memcpy(recive_head,recive_data,4);

	//获取具体type id
	protocol::S2SSystem_GetType Type; 
	Type.ParseFromString(string(recive_head));
	int type_id = Type.pid();

	switch (type_id)
	{
	case ResultInfoID:
		{
			protocol::S2SSystem_ResultInfo result_;
			result_.ParseFromString(string(recive_data));
			if ( result_.result() ==  LOGINSUCCESS){

			}else{
				
			}
		}

		break;
	default:
		break;
	}
}