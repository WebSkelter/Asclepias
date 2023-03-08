/*
* File:		Title.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Sandbox.h"

// Define title scene functions.

bool Title::init() {
	ASCLOG(Title, Info, "Initialized title scene.");
	return true;
}

void Title::enter(Scene& prev) {
	ASCLOG(Title, Info, "Entered title scene from ", &prev, ".");
}

void Title::draw() {

}

bool Title::processInput() {
	return true;
}

void Title::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {

}

void Title::update(float dt) {

}

void Title::leave(Scene& next) {
	ASCLOG(Title, Info, "Left title scene for ", &next, ".");
}

void Title::destroy() {
	ASCLOG(Title, Info, "Destroyed title scene.");
}

// Declare static Sandbox variables.

Title Sandbox::TITLE_SCENE;

// Declare Sandbox entry point.

int main(int argc, char** argv) {
	App::Config conf(Sandbox::TITLE_SCENE);
	if (!App::Init(conf)) {
		return EXIT_FAILURE;
	}
	App::Run();
	App::Destroy();
	return EXIT_SUCCESS;
}