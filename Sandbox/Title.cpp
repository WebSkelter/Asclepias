/*
* File:		Title.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Sandbox.h"

// Define title scene functions.

bool TitleScene::init() {
	// Set UI values.
	glm::vec2 btnDims(0.5f, 0.15f);

	// Create main UI group.
	UI_.init(*this, 0, App::renderer().getShader(), Sandbox::FONT);
	startBtn_.init(glm::vec3(0.25f, 0.5f, 0.0f), btnDims, Sandbox::BTN_TEX,
		"START", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	UI_.addCmpt(startBtn_);
	optionsBtn_.init(glm::vec3(0.25f, 0.3f, 0.0f), btnDims, Sandbox::BTN_TEX,
		"OPTIONS", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	UI_.addCmpt(optionsBtn_);
	exitBtn_.init(glm::vec3(0.25f, 0.1f, 0.0f), btnDims, Sandbox::BTN_TEX,
		"EXIT", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	UI_.addCmpt(exitBtn_);

	ASCLOG(Title, Info, "Initialized title scene.");
	return true;
}

void TitleScene::enter(Scene& prev) {
	App::renderer().getShader().getCamera().pos = glm::vec3(
		App::window().getDims().x / 2, App::window().getDims().y / 2, 0.0f);

	ASCLOG(Title, Info, "Entered title scene from ", &prev, ".");
}

void TitleScene::draw() {
	UI_.draw();
}

bool TitleScene::processInput() {
	UI_.processInput();
	return !exited_;
}

void TitleScene::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {
	if (gID == UI_.getID()) {
		if (cID == startBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::SetScene(Sandbox::GAME);
			}
		}
		else if (cID == optionsBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::SetScene(Sandbox::OPTIONS);
			}
		}
		else if (cID == exitBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				exited_ = true;
			}
		}
	}
}

void TitleScene::update(float dt) {
	UI_.update(dt);
}

void TitleScene::leave(Scene& next) {
	ASCLOG(Title, Info, "Left title scene for ", &next, ".");
}

void TitleScene::destroy() {
	UI_.destroy();

	ASCLOG(Title, Info, "Destroyed title scene.");
}