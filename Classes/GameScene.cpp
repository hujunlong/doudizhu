#include "GameScene.h"

GameScene::GameScene(){
	this->gameLayer = NULL;
};

GameScene::~GameScene(){

};

bool GameScene::init(){
	if(!Scene::init()){
		return false;
	}

	gameLayer = ChoosePlayTypeLayer::create();
	if(gameLayer == NULL){
		return false;
	}
	this->addChild(gameLayer);

	return true;
}
