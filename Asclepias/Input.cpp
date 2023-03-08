/*
* File:		Input.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define InputMgr functions.

	void InputMgr::init() {
		glfwSetKeyCallback(App::window().window_, KeyEvent);
		glfwSetCharCallback(App::window().window_, CharEvent);
		glfwSetCursorPosCallback(App::window().window_, MousePosEvent);
		glfwSetMouseButtonCallback(App::window().window_, MouseBtnEvent);
		glfwSetScrollCallback(App::window().window_, MouseScrollEvent);
		glfwSetJoystickCallback(CtrlEvent);
		ASCLOG(Input, Info, "Added GLFW callback functions.");
		unsigned int count = 0;
		for (int j = 0; j <= GLFW_JOYSTICK_LAST; j++) {
			if (glfwJoystickPresent(j) == GLFW_TRUE) {
				if (glfwJoystickIsGamepad(j) == GLFW_TRUE) {
					connectCtrl(j);
					count++;
				}
			}
		}
		ASCLOG(Input, Info, "Added ", count, " pre-connected controllers.");
	}

	void InputMgr::update() {
		for (std::pair<const int, std::pair<bool, bool>>& k : keys_) {
			k.second.second = k.second.first;
		}
		mousePos_.second = mousePos_.first;
		for (std::pair<const int, std::pair<bool, bool>>& b : mouseBtns_) {
			b.second.second = b.second.first;
		}
		mouseScroll_.second = mouseScroll_.first;
		mouseScroll_.first = glm::vec2(0.0f, 0.0f);
		glfwPollEvents();
		while (!removedCtrls_.empty()) {
			int rc = removedCtrls_.back();
			removedCtrls_.popBack();
			std::map<int, std::pair<GLFWgamepadstate, GLFWgamepadstate>>::iterator it = ctrls_.find(rc);
			if (it != ctrls_.end()) {
				ctrls_.erase(it);
			}
		}
		removedCtrls_.clear();
		for (std::pair<const int, std::pair<GLFWgamepadstate, GLFWgamepadstate>>& c : ctrls_) {
			if (!glfwJoystickPresent(c.first)) {
				continue;
			}
			c.second.second = c.second.first;
			glfwGetGamepadState(c.first, &c.second.first);
			for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; b++) {
				if (c.second.first.buttons[b] != c.second.second.buttons[b]) {
					if (c.second.first.buttons[b] == GLFW_PRESS) {
						for (InputLstr* lstr : lstrs_) {
							lstr->ctrlBtnPressed(c.first, b);
						}
					}
					else if (c.second.first.buttons[b] == GLFW_RELEASE) {
						for (InputLstr* lstr : lstrs_) {
							lstr->ctrlBtnReleased(c.first, b);
						}
					}
				}
			}
			for (int a = 0; a <= GLFW_GAMEPAD_AXIS_LAST; a++) {
				if (c.second.first.axes[a] != c.second.second.axes[a]) {
					for (InputLstr* lstr : lstrs_) {
						lstr->ctrlAxisMoved(c.first, a, c.second.first.axes[a]);
					}
				}
			}
		}
	}

	bool InputMgr::addLstr(InputLstr& lstr) {
		if (std::find(lstrs_.begin(), lstrs_.end(), &lstr) != lstrs_.end()) {
			return false;
		}
		lstrs_.push_back(&lstr);
		ASCLOG(Input, Info, "Added input listener at ", &lstr, ".");
		return true;
	}

	bool InputMgr::removeLstr(InputLstr& lstr) {
		std::vector<InputLstr*>::iterator it = std::find(lstrs_.begin(), lstrs_.end(), &lstr);
		if (it == lstrs_.end()) {
			return false;
		}
		lstrs_.erase(it);
		ASCLOG(Input, Info, "Removed input listener at ", &lstr, ".");
		return true;
	}

	void InputMgr::destroy() {
		lstrs_.clear();
		keys_.clear();
		mouseBtns_.clear();
		ctrls_.clear();
		removedCtrls_.clear();
	}

	bool InputMgr::isKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).first;
	}

	void InputMgr::setMouseEnabled(bool enabled) {
		if (enabled && !mouseEnabled_) {
			if (mouseVisible_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}
		else if (!enabled && mouseEnabled_) {
			glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		mouseEnabled_ = enabled;
	}

	void InputMgr::setMouseVisible(bool visible) {
		if (visible && !mouseVisible_) {
			if (mouseEnabled_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		else if (!visible && mouseVisible_) {
			if (mouseEnabled_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		mouseVisible_ = visible;
	}

	glm::vec2 InputMgr::getMousePos(const Camera& cam) const {
		glm::vec2 mp = getMousePos();
		glm::vec2 wd((float)App::window().getDims().x, (float)App::window().getDims().y);
		mp.x -= wd.x / 2.0f;
		mp.y -= wd.y / 2.0f;
		mp /= cam.scale;
		mp.x += cam.pos.x;
		mp.y += cam.pos.y;
		return mp;
	}

	bool InputMgr::isMouseBtnDown(int btn) const {
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			return false;
		}
		return mouseBtns_.at(btn).first;
	}

	std::vector<int> InputMgr::getCtrlIDs() const {
		std::vector<int> ctrls;
		for (int j = 0; j < GLFW_JOYSTICK_LAST; j++) {
			if (isCtrlConnected(j)) {
				ctrls.push_back(j);
			}
		}
		return ctrls;
	}

	bool InputMgr::isCtrlConnected(int ctrl) const {
		return ctrls_.find(ctrl) != ctrls_.end();
	}

	bool InputMgr::isCtrlBtnDown(int ctrl, int btn) const {
		if (ctrls_.find(ctrl) == ctrls_.end()) {
			return false;
		}
		if (btn < 0 || btn > GLFW_GAMEPAD_BUTTON_LAST) {
			return false;
		}
		return ctrls_.at(ctrl).first.buttons[btn] == GLFW_PRESS;
	}

	bool InputMgr::isCtrlAxisMoved(int ctrl, int axis) const {
		if (ctrls_.find(ctrl) == ctrls_.end()) {
			return false;
		}
		if (axis < 0 || axis > GLFW_GAMEPAD_AXIS_LAST) {
			return false;
		}
		return ctrls_.at(ctrl).first.axes[axis] != ctrls_.at(ctrl).second.axes[axis];
	}

	float InputMgr::getCtrlAxisPos(int ctrl, int axis) const {
		if (ctrls_.find(ctrl) == ctrls_.end()) {
			return false;
		}
		if (axis < 0 || axis > GLFW_GAMEPAD_AXIS_LAST) {
			return false;
		}
		return ctrls_.at(ctrl).first.axes[axis];
	}

	bool InputMgr::wasKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).second;
	}

	bool InputMgr::wasMouseBtnDown(int btn) const {
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			return false;
		}
		return mouseBtns_.at(btn).second;
	}

	bool InputMgr::wasCtrlBtnDown(int ctrl, int btn) const {
		if (ctrls_.find(ctrl) == ctrls_.end()) {
			return false;
		}
		if (btn < 0 || btn > GLFW_GAMEPAD_BUTTON_LAST) {
			return false;
		}
		return ctrls_.at(ctrl).second.buttons[btn] == GLFW_PRESS;
	}

	void InputMgr::pressKey(int key) {
		if (keys_.find(key) == keys_.end()) {
			keys_[key] = std::pair<bool, bool>(true, false);
		}
		keys_[key].first = true;
		for (InputLstr* lstr : lstrs_) {
			lstr->keyPressed(key);
		}
	}

	void InputMgr::releaseKey(int key) {
		if (keys_.find(key) == keys_.end()) {
			keys_[key] = std::pair<bool, bool>(false, true);
		}
		keys_[key].first = false;
		for (InputLstr* lstr : lstrs_) {
			lstr->keyReleased(key);
		}
	}

	void InputMgr::typeChar(char c) {
		for (InputLstr* lstr : lstrs_) {
			lstr->charTyped(c);
		}
	}

	void InputMgr::moveMouse(const glm::vec2& pos) {
		if (!mouseEnabled_) {
			return;
		}
		mousePos_.first = pos;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseMoved(pos);
		}
	}

	void InputMgr::pressMouseBtn(int btn) {
		if (!mouseEnabled_) {
			return;
		}
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			mouseBtns_[btn] = std::pair<bool, bool>(true, false);
		}
		mouseBtns_[btn].first = true;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseBtnPressed(btn);
		}
	}

	void InputMgr::releaseMouseBtn(int btn) {
		if (!mouseEnabled_) {
			return;
		}
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			mouseBtns_[btn] = std::pair<bool, bool>(false, true);
		}
		mouseBtns_[btn].first = false;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseBtnReleased(btn);
		}
	}

	void InputMgr::scrollMouse(const glm::vec2& scroll) {
		if (!mouseEnabled_) {
			return;
		}
		mouseScroll_.first = scroll;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseScrolled(scroll);
		}
	}

	void InputMgr::connectCtrl(int ctrl) {
		if (!glfwJoystickIsGamepad(ctrl)) {
			return;
		}
		if (ctrls_.find(ctrl) != ctrls_.end()) {
			return;
		}
		ctrls_[ctrl] = std::pair<GLFWgamepadstate, GLFWgamepadstate>();
		glfwGetGamepadState(ctrl, &ctrls_[ctrl].first);
		glfwGetGamepadState(ctrl, &ctrls_[ctrl].second);
		for (InputLstr* lstr : lstrs_) {
			lstr->ctrlConnected(ctrl);
		}
	}

	void InputMgr::disconnectCtrl(int ctrl) {
		if (ctrls_.find(ctrl) == ctrls_.end()) {
			return;
		}
		for (InputLstr* lstr : lstrs_) {
			lstr->ctrlDisconnected(ctrl);
		}
		removedCtrls_.pushBack(ctrl);
	}

	void InputMgr::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			App::input().pressKey(key);
		}
		else if (action == GLFW_RELEASE) {
			App::input().releaseKey(key);
		}
	}

	void InputMgr::CharEvent(GLFWwindow* window, unsigned int codepoint) {
		App::input().typeChar((char)codepoint);
	}

	void InputMgr::MousePosEvent(GLFWwindow* window, double x, double y) {
		App::input().moveMouse(glm::vec2(x, App::window().getDims().y - y));
	}

	void InputMgr::MouseBtnEvent(GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS) {
			App::input().pressMouseBtn(button);
		}
		else if (action == GLFW_RELEASE) {
			App::input().releaseMouseBtn(button);
		}
	}

	void InputMgr::MouseScrollEvent(GLFWwindow* window, double x, double y) {
		App::input().scrollMouse(glm::vec2(x, y));
	}

	void InputMgr::CtrlEvent(int jid, int event) {
		if (event == GLFW_CONNECTED) {
			App::input().connectCtrl(jid);
		}
		else if (event == GLFW_DISCONNECTED) {
			App::input().disconnectCtrl(jid);
		}
	}
}