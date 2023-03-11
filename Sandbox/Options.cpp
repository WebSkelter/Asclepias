/*
* File:		Options.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.10
*/

#include <sstream>

#include "Sandbox.h"

// Define global options variables, initialize to Asclepias defaults.

glm::ivec2 OptionsScene::WINDOW_DIMS = glm::ivec2(800, 600);
bool OptionsScene::FULLSCREEN = false;
float OptionsScene::VOLUME = 1.0f;

// Define options.txt reading and writing functions.

void OptionsScene::LoadOptions() {
	std::cout << "Loading options." << std::endl;
	std::ifstream file("options.txt");
	if (!file.is_open()) {
		std::cout << "Failed to open options.txt." << std::endl;
		SaveOptions();
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::size_t colon = line.find(':');
		if (colon == std::string::npos) {
			continue;
		}
		std::string opt = line.substr(0, colon);
		std::string val = line.substr(colon + 1);
		if (opt == "window_dims") {
			std::cout << "Loading windows_dims option: " << val << std::endl;
			std::size_t xp = val.find('x');
			if (xp == std::string::npos) {
				continue;
			}
			int w = std::atoi(val.substr(0, xp).c_str());
			int h = std::atoi(val.substr(xp + 1).c_str());
			WINDOW_DIMS = glm::ivec2(w, h);
		}
		else if (opt == "fullscreen") {
			std::cout << "Loading fullscreen option: " << val << std::endl;
			FULLSCREEN = (bool)std::atoi(val.c_str());
		}
		else if (opt == "volume") {
			std::cout << "Loading volume option: " << val << std::endl;
			VOLUME = (float)std::atof(val.c_str());
		}
	}
	file.close();
}

void OptionsScene::SaveOptions() {
	std::cout << "Saving options." << std::endl;
	std::ofstream file("options.txt");
	if (!file.is_open()) {
		std::cout << "Failed to save options.txt." << std::endl;
		return;
	}
	file << "window_dims:" << WINDOW_DIMS.x << "x" << WINDOW_DIMS.y << std::endl;
	file << "fullscreen:" << FULLSCREEN << std::endl;
	file << "volume:" << VOLUME << std::endl;
	file.close();
}

// Define options scene functions.

bool OptionsScene::init() {
	// Set UI camera position and values.
	App::renderer().getShader().getCamera().pos = glm::vec3(
		App::window().getDims().x / 2, App::window().getDims().y / 2, 0.0f);
	glm::vec2 mainBtnDims(0.25f, 0.1f);
	glm::vec2 optionDims(0.5f, 0.1f);
	float csrWidth = 10.0f;

	// Create main UI group.
	mainUI_.init(*this, 0, App::renderer().getShader(), Sandbox::FONT);
	backBtn_.init(glm::vec3(0.0f, 0.0f, 0.0f), mainBtnDims, Sandbox::BTN_TEX,
		"BACK", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	mainUI_.addCmpt(backBtn_);
	defaultsBtn_.init(glm::vec3(0.375, 0.0f, 0.0f), mainBtnDims, Sandbox::BTN_TEX,
		"DEFAULTS", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	mainUI_.addCmpt(defaultsBtn_);
	applyBtn_.init(glm::vec3(0.75f, 0.0f, 0.0f), mainBtnDims, Sandbox::BTN_TEX,
		"APPLY", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR);
	mainUI_.addCmpt(applyBtn_);

	// Create options UI group.
	optionsUI_.init(*this, 1, App::renderer().getShader(), Sandbox::FONT);
	windowDimsCyc_.init(glm::vec3(0.25f, 0.8f, 0.0f), optionDims, Sandbox::CYC_TEX,
		Sandbox::CYC_BTN_TEX, { "800x600", "1080x720", "1920x1080" }, "RESOLUTION",
		"", "", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR, ASC_ALIGN_CENTER,
		ASC_ALIGN_CENTER, ASC_ALIGN_RIGHT, ASC_ALIGN_TOP, ASC_ALIGN_LEFT,
		ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(windowDimsCyc_);
	fullscreenSw_.init(glm::vec3(0.25f, 0.6f, 0.0f), optionDims, Sandbox::SW_TEX,
		"FULLSCREEN", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR, ASC_ALIGN_TOP,
		ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(fullscreenSw_);
	volumeSl_.init(glm::vec3(0.25f, 0.4f, 0.0f), optionDims, Sandbox::SL_TEX,
		Sandbox::CSR_TEX, csrWidth, "VOLUME", Sandbox::TEXT_SCALE, Sandbox::TEXT_COLOR,
		ASC_ALIGN_TOP, ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(volumeSl_);

	ASCLOG(Options, Info, "Initialized options scene.");
	return true;
}

void OptionsScene::enter(Scene& prev) {
	// Re-load options.
	std::ostringstream oss;
	oss << WINDOW_DIMS.x << "x" << WINDOW_DIMS.y;
	windowDimsCyc_.setText(oss.str());
	fullscreenSw_.setOn(FULLSCREEN);
	volumeSl_.setValue(VOLUME);

	// Set camera position.
	App::renderer().getShader().getCamera().pos = glm::vec3(
		App::window().getDims().x / 2, App::window().getDims().y / 2, 0.0f);

	ASCLOG(Options, Info, "Entered options scene from ", &prev, ".");
}

void OptionsScene::draw() {
	mainUI_.draw();
	optionsUI_.draw();
}

bool OptionsScene::processInput() {
	mainUI_.processInput();
	optionsUI_.processInput();
	return true;
}

void OptionsScene::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {
	if (gID == mainUI_.getID()) {
		if (cID == backBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::SetScene(Sandbox::TITLE);
			}
		}
		else if (cID == defaultsBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				windowDimsCyc_.setText("800x600");
				fullscreenSw_.setOn(false);
				volumeSl_.setValue(1.0f);
			}
		}
		else if (cID == applyBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				// Get and save option values.
				std::string winDims = windowDimsCyc_.getValue();
				std::size_t splitIndex = winDims.find('x');
				if (splitIndex != std::string::npos) {
					int w = std::atoi(winDims.substr(0, splitIndex).c_str());
					int h = std::atoi(winDims.substr(splitIndex + 1).c_str());
					WINDOW_DIMS = glm::ivec2(w, h);
				}
				else {
					WINDOW_DIMS = glm::ivec2(800, 600);
				}
				FULLSCREEN = fullscreenSw_.isOn();
				VOLUME = volumeSl_.getValue();
				SaveOptions();
				// Apply options to current runtime.
				App::window().setDims(WINDOW_DIMS);
				App::window().setFullscreen(FULLSCREEN);
				App::audio().setVolume(VOLUME);
				// Set camera position.
				App::renderer().getShader().getCamera().pos = glm::vec3(
					App::window().getDims().x / 2, App::window().getDims().y / 2, 0.0f);
			}
		}
	}
}

void OptionsScene::update(float dt) {
	mainUI_.update(dt);
	optionsUI_.update(dt);
	App::renderer().getShader().getCamera().update(dt);
}

void OptionsScene::leave(Scene& next) {
	ASCLOG(Options, Info, "Left options scene for ", &next, ".");
}

void OptionsScene::destroy() {
	mainUI_.destroy();
	optionsUI_.destroy();

	ASCLOG(Options, Info, "Destroyed options scene.");
}