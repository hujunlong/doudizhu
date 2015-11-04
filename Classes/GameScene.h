#pragma 

#include "cocos2d.h"
#include "GameLayer.h"
 

using namespace cocos2d;
using namespace CocosDenshion;

class GameScene : public Scene{
public:
	GameScene(void);

	~GameScene(void);

	virtual bool init();

	CREATE_FUNC(GameScene);
public:
	GameLayer* gameLayer;

};