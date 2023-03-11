/*
* File:		Sandbox.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_SANDBOX_H
#define ASC_SANDBOX_H

#include <Asclepias/Asclepias.h>

using namespace ASC;

// Declare all scene classes.

class TitleScene : public Scene {
public:
	// Overridden scene functions.
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;

private:
	// Main UI group.
	UIGroup UI_;
	Button startBtn_;
	Button optionsBtn_;
	Button exitBtn_;
	// Set if exit button is clicked.
	bool exited_ = false;
};

class OptionsScene : public Scene {
public:
	// Global options variables.
	static glm::ivec2 WINDOW_DIMS;
	static bool FULLSCREEN;
	static float VOLUME;

	// Attempts to read options.txt.
	static void LoadOptions();

	// Overridden scene functions.
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;

private:
	// Main UI group on bottom.
	UIGroup mainUI_;
	Button backBtn_;
	Button defaultsBtn_;
	Button applyBtn_;
	// Options UI group for changing values.
	UIGroup optionsUI_;
	Cycle windowDimsCyc_;
	Switch fullscreenSw_;
	Slider volumeSl_;

	// Writes options.txt.
	static void SaveOptions();
};

class GameScene : public Scene {
public:
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;
};

// Declare static Sandbox application class.

class Sandbox {
public:
	// Global variables.
	static Font& FONT;
	static GLuint BTN_TEX;
	static GLuint CYC_TEX;
	static GLuint CYC_BTN_TEX;
	static GLuint SW_TEX;
	static GLuint SL_TEX;
	static GLuint CSR_TEX;
	static float TEXT_SCALE;
	static glm::vec3 TEXT_COLOR;
	// Scenes.
	static TitleScene TITLE;
	static OptionsScene OPTIONS;
	static GameScene GAME;
};

#endif