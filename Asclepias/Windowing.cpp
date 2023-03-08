/*
* File:		Windowing.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define WindowMgr functions.

	bool WindowMgr::init(const glm::ivec2& dims, const std::string& title, bool fullscreen) {
		if (window_ != 0) {
			ASCLOG(Window, Warning, "Window already initialized.");
			return false;
		}
		if (!glfwInit()) {
			ASCLOG(Window, Error, "Failed to initialize GLFW library.");
			return false;
		}
		ASCLOG(Window, Info, "Initialized GLFW library.");
		monitor_ = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		if (!fullscreen) {
			window_ = glfwCreateWindow(dims.x, dims.y, title.c_str(), 0, 0);
		}
		else {
			const GLFWvidmode* vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
			window_ = glfwCreateWindow(vm->width, vm->height, title.c_str(), monitor_, 0);
		}
		if (!window_) {
			ASCLOG(Window, Error, "Failed to open GLFW window.");
			return false;
		}
		ASCLOG(Window, Info, "Created GLFW window.");
		setFullscreen(fullscreen);
		setDims(dims);
		setTitle(title);
		glfwMakeContextCurrent(window_);
		center();
		return true;
	}

	bool WindowMgr::update() {
		glfwSwapBuffers(window_);
		return !glfwWindowShouldClose(window_);
	}

	void WindowMgr::destroy() {
		if (!window_) {
			ASCLOG(Window, Warning, "Window already destroyed.");
			return;
		}
		glfwDestroyWindow(window_);
		glfwTerminate();
		ASCLOG(Window, Info, "Terminated GLFW library.");
		dims_ = glm::ivec2();
		wDims_ = glm::ivec2();
		title_ = "";
		fullscreen_ = false;
		monitor_ = 0;
		window_ = 0;
	}

	void WindowMgr::setDims(const glm::ivec2& dims) {
		wDims_ = dims;
		if (fullscreen_) {
			return;
		}
		dims_ = dims;
		glfwSetWindowSize(window_, dims.x, dims.y);
		glViewport(0, 0, dims.x, dims.y);
		center();
	}

	void WindowMgr::setTitle(const std::string& title) {
		title_ = title;
		glfwSetWindowTitle(window_, title.c_str());
	}

	void WindowMgr::setFullscreen(bool fullscreen) {
		if (fullscreen && !fullscreen_) {
			const GLFWvidmode* vm = glfwGetVideoMode(monitor_);
			glfwSetWindowMonitor(window_, monitor_, 0, 0, vm->width, vm->height, GLFW_DONT_CARE);
			glViewport(0, 0, vm->width, vm->height);
			wDims_ = dims_;
			dims_ = glm::ivec2(vm->width, vm->height);
		}
		else if (!fullscreen && fullscreen_) {
			glfwSetWindowMonitor(window_, 0, 0, 0, wDims_.x, wDims_.y, GLFW_DONT_CARE);
			glfwSetWindowAttrib(window_, GLFW_DECORATED, GLFW_TRUE);
			glViewport(0, 0, wDims_.x, wDims_.y);
			dims_ = wDims_;
			center();
		}
		fullscreen_ = fullscreen;
	}

	void WindowMgr::setMonitor(GLFWmonitor* monitor) {
		monitor_ = monitor;
	}

	void WindowMgr::center() {
		const GLFWvidmode* vm = glfwGetVideoMode(monitor_);
		glfwSetWindowPos(window_, (vm->width - dims_.x) / 2, (vm->height - dims_.y) / 2);
	}
}