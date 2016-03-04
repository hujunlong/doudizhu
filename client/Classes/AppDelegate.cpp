#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

ODSocket cSocket;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Poker");
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(800,600,ResolutionPolicy::NO_BORDER);
    

	setResourceSearchResolution();

	createNetWork();

	 // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene =GameScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::setResourceSearchResolution(){ //用来添加Cocostudio所创建的资源
	std::vector<std::string> paths;
	paths.push_back("login");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
}

void AppDelegate::createNetWork(){
	cSocket.Init();
	cSocket.Create(AF_INET,SOCK_STREAM,0);
	cSocket.Connect("127.0.0.1",8087);
}