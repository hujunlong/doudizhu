#ifndef __TextFieldTTF_H__
#define __TextFieldTTF_H__

#include "cocos2d.h"
USING_NS_CC;

class MyTextFieldTTF : public CCLayer,public CCTextFieldDelegate
{
public:
	bool init();  
	bool onTouchBegan(CCTouch*, CCEvent*);
	void CreateText(const char *tip,const Vec2& position);
	CREATE_FUNC(MyTextFieldTTF);
private:
	CCTextFieldTTF* textEdit;
};

#endif // __HELLOWORLD_SCENE_H__
