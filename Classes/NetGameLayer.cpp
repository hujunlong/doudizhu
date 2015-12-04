#include "NetGameLayer.h"
 
NetGameLayer::NetGameLayer(){
	
}

NetGameLayer::~NetGameLayer(){
	
}

bool NetGameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	
	initBackGround();
	CreateNetWork();
	GetLoginInfo();
	 return true; 
}
 

bool NetGameLayer::initBackGround(){
	bool isRet = false;
	CCSprite *bk = CCSprite::create("bk.png");
	this->addChild(bk,0);
	bk->setAnchorPoint(ccp(0,0));
	isRet = true;
	return isRet;
}

void NetGameLayer::CreateNetWork(){
	cSocket.Init();
	cSocket.Create(AF_INET,SOCK_STREAM,0);
	cSocket.Connect("127.0.0.1",7880);
}

bool NetGameLayer::GetLoginInfo(){
	std::fstream file_;
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath();
	path += "/login.txt";
	file_.open(path,std::ios::in);

	if (!file_)//不存在 先创建注册
	{
		auto s = Director::getInstance()->getWinSize();
		//用户名
		auto text_name = TextFieldTTF::textFieldWithPlaceHolder("<click here for name>",
			"Thonburi.ttf",30);
		text_name->setPosition(Vec2(s.width / 2-50, s.height / 2 + 30));
		this->addChild(text_name);
		setTouchMode(kCCTouchesOneByOne);
		setTouchEnabled(true);

		//密码
		auto text_pwd = TextFieldTTF::textFieldWithPlaceHolder("<click here for pwd>",
			"Thonburi.ttf",30);
		text_pwd->setPosition(Vec2(s.width / 2 - 50, s.height / 2 ));
		this->addChild(text_pwd);

	}else{//读取文件的name pwd
		std::ifstream infile;
		infile>>login_info.name;
		infile>>login_info.pwd;
		return true;
	}
	return false;
}