/*
* File:		Sandbox.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.10
*/

#include "Sandbox.h"

// Define global Sandbox variables.

Font& Sandbox::FONT = Renderer::NO_FONT;
GLuint Sandbox::BTN_TEX = Renderer::NO_TEXTURE;
GLuint Sandbox::CYC_TEX = Renderer::NO_TEXTURE;
GLuint Sandbox::CYC_BTN_TEX = Renderer::NO_TEXTURE;
GLuint Sandbox::SW_TEX = Renderer::NO_TEXTURE;
GLuint Sandbox::SL_TEX = Renderer::NO_TEXTURE;
GLuint Sandbox::CSR_TEX = Renderer::NO_TEXTURE;
float Sandbox::TEXT_SCALE = 0.65f;
glm::vec3 Sandbox::TEXT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

// Define Sandbox scenes.

TitleScene Sandbox::TITLE;
OptionsScene Sandbox::OPTIONS;
GameScene Sandbox::GAME;

// Define Sandbox entry point.

int main(int argc, char** argv) {
	// Load options and configure Asclepias App.
	OptionsScene::LoadOptions();
	App::Config conf(Sandbox::TITLE);
	conf.window.dims = OptionsScene::WINDOW_DIMS;
	conf.window.fullscreen = OptionsScene::FULLSCREEN;
	conf.audio.volume = OptionsScene::VOLUME;
	if (!App::Init(conf)) {
		return EXIT_FAILURE;
	}

	// Load global values.
	Sandbox::FONT = App::renderer().getFont("Assets/fonts/cour.ttf");
	Sandbox::BTN_TEX = App::renderer().getTexture("Assets/images/button.png");
	Sandbox::CYC_TEX = App::renderer().getTexture("Assets/images/cycle.png");
	Sandbox::CYC_BTN_TEX = App::renderer().getTexture("Assets/images/cycle_button.png");
	Sandbox::SW_TEX = App::renderer().getTexture("Assets/images/switch.png");
	Sandbox::SL_TEX = App::renderer().getTexture("Assets/images/slider.png");
	Sandbox::CSR_TEX = App::renderer().getTexture("Assets/images/slider_cursor.png");

	// Run Asclepias App.
	App::Run();
	App::Destroy();
	return EXIT_SUCCESS;
}