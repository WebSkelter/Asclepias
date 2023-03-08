/*
* File:		Windowing.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_WINDOWING_H
#define ASC_WINDOWING_H

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ASC {
	// Application utility for opening windows via GLFW.
	class WindowMgr {
	public:
		/*
		* Initialize this window manager's memory.
		* @param const glm::ivec2& dims: The dimensions of ths window (w, h).
		* @param const std::string& title: The title of the window.
		* @param bool fullscreen: Whether to open this window in fullscreen mode.
		* @return Whether GLFW could be initialize, and the window opened.
		*/
		bool init(const glm::ivec2&, const std::string&, bool);
		/*
		* Swap the window's frame buffer.
		* @return Whether this window should remain open.
		*/
		bool update();
		/*
		* Free this window manager's memory.
		*/
		void destroy();
		/*
		* @return The dimensions of the window (w, h).
		*/
		inline const glm::ivec2& getDims() const {
			return dims_;
		}
		/*
		* @param const glm::ivec2& dims: The dimensions of the window (w, h).
		*/
		void setDims(const glm::ivec2&);
		/*
		* @return The title of the window.
		*/
		inline const std::string& getTitle() const {
			return title_;
		}
		/*
		* @param const std::string& title: The title of the window.
		*/
		void setTitle(const std::string&);
		/*
		* @return Whether this window is in fullscreen mode.
		*/
		inline bool isFullscreen() const {
			return fullscreen_;
		}
		/*
		* @param bool fullscreen: Whether to set this window to fullscreen mode.
		*/
		void setFullscreen(bool);
		/*
		* @param GLFWmonitor* monitor: The monitor retrieved by GLFW to use for the window.
		*/
		void setMonitor(GLFWmonitor*);

	private:
		friend class InputMgr;

		// The current dimensions of this window (w, h).
		glm::ivec2 dims_ = glm::ivec2();
		// The dimensions of this window in windowed mode.
		glm::ivec2 wDims_ = glm::ivec2();
		// The title of this window.
		std::string title_ = "";
		// Whether this window is in fullscreen mode.
		bool fullscreen_ = false;
		// The current monitor for this window.
		GLFWmonitor* monitor_ = 0;
		// The GLFW handle for this window.
		GLFWwindow* window_ = 0;

		/*
		* Center the window on the screen.
		*/
		void center();
	};
}

#endif