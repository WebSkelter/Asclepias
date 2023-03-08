/*
* File:		Sandbox.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_SANDBOX_H
#define ASC_SANDBOX_H

#include <Asclepias/Asclepias.h>

using namespace ASC;

// Initial scene for Sandbox App.
class Title : public Scene {
public:
	/*
	* Initialize title scene.
	* @return Whether init succeeded.
	*/
	bool init() override;
	/*
	* Enter the title scene from another.
	* @param Scene& prev: The previous scene.
	*/
	void enter(Scene&) override;
	/*
	* Draw the title scene's graphics.
	*/
	void draw() override;
	/*
	* Process input to the title scene.
	*/
	bool processInput() override;
	/*
	* Process UI component events on the title scene.
	* @param unsigned int gID: The UI group ID.
	* @param unsigned int cID: The UI component ID.
	* @param unsigned int eID: The UI event ID.
	*/
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	/*
	* Update the title scene's physics.
	* @param float dt: Delta-time.
	*/
	void update(float) override;
	/*
	* Leave the title scene for another.
	* @param Scene& next: The next scene.
	*/
	void leave(Scene&) override;
	/*
	* Free the title scene's memory.
	*/
	void destroy() override;
};

// Main class of the Sandbox App.
class Sandbox {
public:
	// Initial title scene.
	static Title TITLE_SCENE;
};

#endif