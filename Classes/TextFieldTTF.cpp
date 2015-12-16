#include "TextFieldTTF.h"


bool MyTextFieldTTF::init()
{
	CCLayer::init();
	return true;
}

void MyTextFieldTTF::CreateText(const char *tip,const Vec2& position){
	textEdit = CCTextFieldTTF::textFieldWithPlaceHolder(tip,"Arial", 36);
	textEdit->setPosition(position);
	addChild(textEdit);

	setTouchMode(kCCTouchesOneByOne);
	setTouchEnabled(true);
}


bool MyTextFieldTTF::onTouchBegan(CCTouch* touch, CCEvent* ev)
{
	bool isClicked = textEdit->boundingBox().containsPoint(touch->getLocation());
	if(isClicked){
		//µ¯³öÈí¼üÅÌ
		textEdit->attachWithIME();
	}
	return true;
}
