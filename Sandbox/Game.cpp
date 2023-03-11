/*
* File:		Game.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.10
*/

#include "Sandbox.h"

bool GameScene::init() {
	ASCLOG(Game, Info, "Initialized game scene.");
	return true;
}

void GameScene::enter(Scene& prev) {
	ASCLOG(Game, Info, "Entered game scene from ", &prev, ".");
}

void GameScene::draw() {

}

bool GameScene::processInput() {
	if (App::input().isKeyPressed(ASC_CTL_ESC)) {
		App::SetScene(Sandbox::TITLE);
	}
	return true;
}

void GameScene::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {

}

void GameScene::update(float dt) {

}

void GameScene::leave(Scene& next) {
	ASCLOG(Game, Info, "Left game scene for ", &next, ".");
}

void GameScene::destroy() {
	ASCLOG(Game, Info, "Destroyed game scene.");
}