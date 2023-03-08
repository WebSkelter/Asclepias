/*
* File:		Input.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_INPUT_H
#define ASC_INPUT_H

#include <mutex>
#include <deque>
#include <map>
#include <vector>

#include "Windowing.h"

namespace ASC {
	// Wrappers for all GLFW input codes.
	enum InputCodes {
		ASC_UNKNOWN = GLFW_KEY_UNKNOWN,

		ASC_SYM_GRAVE = GLFW_KEY_GRAVE_ACCENT, ASC_SYM_MINUS = GLFW_KEY_MINUS,
		ASC_SYM_EQUALS = GLFW_KEY_EQUAL, ASC_SYM_L_BRACKET = GLFW_KEY_LEFT_BRACKET,
		ASC_SYM_R_BRACKET = GLFW_KEY_RIGHT_BRACKET, ASC_SYM_BACKSLASH = GLFW_KEY_BACKSLASH,
		ASC_SYM_SEMICOLON = GLFW_KEY_SEMICOLON, ASC_SYM_APOSTROPHE = GLFW_KEY_APOSTROPHE,
		ASC_SYM_COMMA = GLFW_KEY_COMMA, ASC_SYM_PERIOD = GLFW_KEY_PERIOD,
		ASC_SYM_SLASH = GLFW_KEY_SLASH, ASC_SYM_SPACE = GLFW_KEY_SPACE,
		ASC_SYM_WORLD_1 = GLFW_KEY_WORLD_1, ASC_SYM_WORLD_2 = GLFW_KEY_WORLD_2,
		ASC_SYM_KP_DECIMAL = GLFW_KEY_KP_DECIMAL, ASC_SYM_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		ASC_SYM_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY, ASC_SYM_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		ASC_SYM_KP_ADD = GLFW_KEY_KP_ADD, ASC_SYM_KP_ENTER = GLFW_KEY_KP_ENTER,
		ASC_SYM_KP_EQUALS = GLFW_KEY_KP_EQUAL,

		ASC_KEY_0 = GLFW_KEY_0, ASC_KEY_1 = GLFW_KEY_1, ASC_KEY_2 = GLFW_KEY_2,
		ASC_KEY_3 = GLFW_KEY_3, ASC_KEY_4 = GLFW_KEY_4, ASC_KEY_5 = GLFW_KEY_5,
		ASC_KEY_6 = GLFW_KEY_6, ASC_KEY_7 = GLFW_KEY_7, ASC_KEY_8 = GLFW_KEY_8,
		ASC_KEY_9 = GLFW_KEY_9,
		ASC_KEY_KP_0 = GLFW_KEY_KP_0, ASC_KEY_KP_1 = GLFW_KEY_KP_1, ASC_KEY_KP_2 = GLFW_KEY_KP_2,
		ASC_KEY_KP_3 = GLFW_KEY_KP_3, ASC_KEY_KP_4 = GLFW_KEY_KP_4, ASC_KEY_KP_5 = GLFW_KEY_KP_5,
		ASC_KEY_KP_6 = GLFW_KEY_KP_6, ASC_KEY_KP_7 = GLFW_KEY_KP_7, ASC_KEY_KP_8 = GLFW_KEY_KP_8,
		ASC_KEY_KP_9 = GLFW_KEY_KP_9,

		ASC_KEY_A = GLFW_KEY_A, ASC_KEY_B = GLFW_KEY_B, ASC_KEY_C = GLFW_KEY_C,
		ASC_KEY_D = GLFW_KEY_D, ASC_KEY_E = GLFW_KEY_E, ASC_KEY_F = GLFW_KEY_F,
		ASC_KEY_G = GLFW_KEY_G, ASC_KEY_H = GLFW_KEY_H, ASC_KEY_I = GLFW_KEY_I,
		ASC_KEY_J = GLFW_KEY_J, ASC_KEY_K = GLFW_KEY_K, ASC_KEY_L = GLFW_KEY_L,
		ASC_KEY_M = GLFW_KEY_M, ASC_KEY_N = GLFW_KEY_N, ASC_KEY_O = GLFW_KEY_O,
		ASC_KEY_P = GLFW_KEY_P, ASC_KEY_Q = GLFW_KEY_Q, ASC_KEY_R = GLFW_KEY_R,
		ASC_KEY_S = GLFW_KEY_S, ASC_KEY_T = GLFW_KEY_T, ASC_KEY_U = GLFW_KEY_U,
		ASC_KEY_V = GLFW_KEY_V, ASC_KEY_W = GLFW_KEY_W, ASC_KEY_X = GLFW_KEY_X,
		ASC_KEY_Y = GLFW_KEY_Y, ASC_KEY_Z = GLFW_KEY_Z,

		ASC_CTL_ESC = GLFW_KEY_ESCAPE, ASC_CTL_ENTER = GLFW_KEY_ENTER, ASC_CTL_TAB = GLFW_KEY_TAB,
		ASC_CTL_BACKSPACE = GLFW_KEY_BACKSPACE, ASC_CTL_INSERT = GLFW_KEY_INSERT,
		ASC_CTL_DELETE = GLFW_KEY_DELETE, ASC_CTL_LEFT = GLFW_KEY_LEFT, ASC_CTL_RIGHT = GLFW_KEY_RIGHT,
		ASC_CTL_UP = GLFW_KEY_UP, ASC_CTL_DOWN = GLFW_KEY_DOWN, ASC_CTL_PG_UP = GLFW_KEY_PAGE_UP,
		ASC_CTL_PG_DOWN = GLFW_KEY_PAGE_DOWN, ASC_CTL_HOME = GLFW_KEY_HOME, ASC_CTL_END = GLFW_KEY_END,
		ASC_CTL_CAPS_LOCK = GLFW_KEY_CAPS_LOCK, ASC_CTL_PRINTSC = GLFW_KEY_PRINT_SCREEN,
		ASC_CTL_PAUSE = GLFW_KEY_PAUSE,
		ASC_CTL_F1 = GLFW_KEY_F1, ASC_CTL_F2 = GLFW_KEY_F2, ASC_CTL_F3 = GLFW_KEY_F3,
		ASC_CTL_F4 = GLFW_KEY_F4, ASC_CTL_F5 = GLFW_KEY_F5, ASC_CTL_F6 = GLFW_KEY_F6,
		ASC_CTL_F7 = GLFW_KEY_F7, ASC_CTL_F8 = GLFW_KEY_F8, ASC_CTL_F9 = GLFW_KEY_F9,
		ASC_CTL_F10 = GLFW_KEY_F10, ASC_CTL_F11 = GLFW_KEY_F11, ASC_CTL_F12 = GLFW_KEY_F12,
		ASC_CTL_F13 = GLFW_KEY_F13, ASC_CTL_F14 = GLFW_KEY_F14, ASC_CTL_F15 = GLFW_KEY_F15,
		ASC_CTL_F16 = GLFW_KEY_F16, ASC_CTL_F17 = GLFW_KEY_F17, ASC_CTL_F18 = GLFW_KEY_F18,
		ASC_CTL_F19 = GLFW_KEY_F19, ASC_CTL_F20 = GLFW_KEY_F20, ASC_CTL_F21 = GLFW_KEY_F21,
		ASC_CTL_F22 = GLFW_KEY_F22, ASC_CTL_F23 = GLFW_KEY_F23, ASC_CTL_F24 = GLFW_KEY_F24,
		ASC_CTL_F25 = GLFW_KEY_F25,
		ASC_CTL_L_SHIFT = GLFW_KEY_LEFT_SHIFT, ASC_CTL_L_CTRL = GLFW_KEY_LEFT_CONTROL,
		ASC_CTL_L_ALT = GLFW_KEY_LEFT_ALT, ASC_CTL_L_SUPER = GLFW_KEY_LEFT_SUPER,
		ASC_CTL_R_SHIFT = GLFW_KEY_RIGHT_SHIFT, ASC_CTL_R_CTRL = GLFW_KEY_RIGHT_CONTROL,
		ASC_CTL_R_ALT = GLFW_KEY_RIGHT_ALT, ASC_CTL_R_SUPER = GLFW_KEY_RIGHT_SUPER,
		ASC_CTL_MENU = GLFW_KEY_MENU, ASC_CTL_LAST = GLFW_KEY_LAST,

		ASC_MB_1 = GLFW_MOUSE_BUTTON_1, ASC_MB_2 = GLFW_MOUSE_BUTTON_2, ASC_MB_3 = GLFW_MOUSE_BUTTON_3,
		ASC_MB_4 = GLFW_MOUSE_BUTTON_4, ASC_MB_5 = GLFW_MOUSE_BUTTON_5, ASC_MB_6 = GLFW_MOUSE_BUTTON_6,
		ASC_MB_7 = GLFW_MOUSE_BUTTON_7, ASC_MB_8 = GLFW_MOUSE_BUTTON_8,
		ASC_MB_LEFT = GLFW_MOUSE_BUTTON_LEFT, ASC_MB_MID = GLFW_MOUSE_BUTTON_MIDDLE,
		ASC_MB_RIGHT = GLFW_MOUSE_BUTTON_RIGHT, ASC_MB_LAST = GLFW_MOUSE_BUTTON_LAST,

		ASC_CB_A = GLFW_GAMEPAD_BUTTON_A, ASC_CB_B = GLFW_GAMEPAD_BUTTON_B,
		ASC_CB_X = GLFW_GAMEPAD_BUTTON_X, ASC_CB_Y = GLFW_GAMEPAD_BUTTON_Y,
		ASC_CB_CROSS = GLFW_GAMEPAD_BUTTON_CROSS, ASC_CB_CIRCLE = GLFW_GAMEPAD_BUTTON_CIRCLE,
		ASC_CB_SQUARE = GLFW_GAMEPAD_BUTTON_SQUARE, ASC_CB_TRIANGLE = GLFW_GAMEPAD_BUTTON_TRIANGLE,
		ASC_CB_L_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, ASC_CB_R_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		ASC_CB_L_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB, ASC_CB_R_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		ASC_CB_DP_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP, ASC_CB_DP_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
		ASC_CB_DP_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, ASC_CB_DP_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
		ASC_CB_BACK = GLFW_GAMEPAD_BUTTON_BACK, ASC_CB_START = GLFW_GAMEPAD_BUTTON_START,
		ASC_CB_GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE, ASC_CB_LAST = GLFW_GAMEPAD_BUTTON_LAST,

		ASC_CA_L_STICK_X = GLFW_GAMEPAD_AXIS_LEFT_X, ASC_CA_L_STICK_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
		ASC_CA_R_STICK_X = GLFW_GAMEPAD_AXIS_RIGHT_X, ASC_CA_R_STICK_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
		ASC_CA_L_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, ASC_CA_R_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
		ASC_CA_LAST = GLFW_GAMEPAD_AXIS_LAST,
	};

	// Abstract interface for user input callbacks.
	class InputLstr {
	public:
		/*
		* A keyboard key has been pressed.
		* @param int key: The key ID.
		*/
		virtual void keyPressed(int) {}
		/*
		* A keyboard key has been released.
		* @param int key: The key ID.
		*/
		virtual void keyReleased(int) {}
		/*
		* A character has been typed on the keyboard.
		* @param char c: The character.
		*/
		virtual void charTyped(char) {}
		/*
		* The mouse cursor has moved on the window.
		* @param const glm::vec2& mp: The new mouse position.
		*/
		virtual void mouseMoved(const glm::vec2&) {}
		/*
		* A mouse button has been pressed.
		* @param int btn: The button ID.
		*/
		virtual void mouseBtnPressed(int) {}
		/*
		* A mouse button has been released.
		* @param int btn: The button ID.
		*/
		virtual void mouseBtnReleased(int) {}
		/*
		* The mouse scroll wheel has moved.
		* @param const glm::vec2& scroll: The scroll distance.
		*/
		virtual void mouseScrolled(const glm::vec2&) {}
		/*
		* A game controller has connected.
		* @param int ctrl: The controller ID.
		*/
		virtual void ctrlConnected(int) {}
		/*
		* A game controller has disconnected.
		* @param int ctrl: The controller ID.
		*/
		virtual void ctrlDisconnected(int) {}
		/*
		* A game controller button has been pressed.
		* @param int ctrl: The controller ID.
		* @param int btn: The button ID.
		*/
		virtual void ctrlBtnPressed(int, int) {}
		/*
		* A game controller button has been released.
		* @param int ctrl: The controller ID.
		* @param int btn: The button ID.
		*/
		virtual void ctrlBtnReleased(int, int) {}
		/*
		* A game controller axis has moved.
		* @param int ctrl: The controller ID.
		* @param int axis: The axis ID.
		* @param float pos: The new position of the axis.
		*/
		virtual void ctrlAxisMoved(int, int, float) {}
	};

	/*
	* Thread-safe double-ended queue structure.
	*/
	template <typename T>
	class TSQueue {
	public:
		/*
		* Construct an empty TSQueue.
		*/
		TSQueue() = default;
		/*
		* Do not copy TSQueue's.
		* @param const TSQueue<T>& q: The queue not to copy.
		*/
		TSQueue(const TSQueue<T>&) = delete;
		/*
		* Free the memory of a TSQueue.
		*/
		~TSQueue() {
			clear();
		}
		/*
		* Clear this TSQueue's elements.
		*/
		void clear() {
			std::scoped_lock(mtx_);
			dequeue_.clear();
		}
		/*
		* @return The number of elements in this TSQueue.
		*/
		unsigned int size() const {
			std::scoped_lock(mtx_);
			return (unsigned int)dequeue_.size();
		}
		/*
		* @return Whether this TSQueue contains no elements.
		*/
		bool empty() {
			std::scoped_lock(mtx_);
			return dequeue_.empty();
		}
		/*
		* Get the element of this TSQueue at the given index.
		* @param unsigned int i: The index to get.
		* @return The element of this TSQueue at the index i.
		*/
		T& at(unsigned int i) {
			std::scoped_lock(mtx_);
			return dequeue_.at(i);
		}
		/*
		* @return The element at the front of this TSQueue.
		*/
		const T& front() const {
			std::scoped_lock(mtx_);
			return dequeue_.front();
		}
		/*
		* @return The element at the back of this TSQueue.
		*/
		const T& back() const {
			std::scoped_lock(mtx_);
			return dequeue_.back();
		}
		/*
		* Get and remove an element from the front of this TSQueue.
		* @return The element at the front.
		*/
		T popFront() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.front());
			dequeue_.pop_front();
			return e;
		}
		/*
		* Get and remove an element from the back of this TSQueue.
		* @return The element at the back.
		*/
		T popBack() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.back());
			dequeue_.pop_back();
			return e;
		}
		/*
		* Add an element to the front of this TSQueue.
		* @param const T& e: The element to add.
		*/
		void pushFront(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_front(std::move(e));
		}
		/*
		* Add an element to the back of this TSQueue.
		* @param const T& e: The element to add.
		*/
		void pushBack(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_back(std::move(e));
		}

	private:
		// The double-ended queue this TSQueue wraps.
		std::deque<T> dequeue_;
		// Mutex used to give this TSQueue thread-safety.
		std::mutex mtx_;
	};

	// App utility for managing user input from keyboard, mouse and controllers.
	class InputMgr {
	public:
		/*
		* Initialize this input manager's memory and set up GLFW callbacks.
		*/
		void init();
		/*
		* Update this input manager's logic and state.
		*/
		void update();
		/*
		* Add an input listener to this input manager.
		* @param InputLstr& lstr: The listener to add.
		* @return Whether the listener could be added.
		*/
		bool addLstr(InputLstr&);
		/*
		* Remove an input listener from this input manager.
		* @param InputLstr& lstr: The listener to remove.
		* @return Whether the listener could be removed.
		*/
		bool removeLstr(InputLstr&);
		/*
		* Free this input manager's memory.
		*/
		void destroy();
		/*
		* Determine whether a keyboard key is down.
		* @param int key: The key ID to test.
		* @return Whether the key is currently down.
		*/
		bool isKeyDown(int) const;
		/*
		* Determine whether a keyboard key has just been pressed.
		* @param int key: The key ID to test.
		* @return Whether the key has been pressed.
		*/
		inline bool isKeyPressed(int key) const {
			return isKeyDown(key) && !wasKeyDown(key);
		}
		/*
		* Determine whether a keyboard key has just been released.
		* @param int key: The key ID to test.
		* @return Whether the key has been released.
		*/
		inline bool isKeyReleased(int key) const {
			return !isKeyDown(key) && wasKeyDown(key);
		}
		/*
		* @return Whether the mouse cursor is enabled on the window.
		*/
		inline bool isMouseEnabled() const {
			return mouseEnabled_;
		}
		/*
		* @param bool enabled: Whether the mouse cursor is enabled on the window.
		*/
		void setMouseEnabled(bool);
		/*
		* @return Whether the mouse cursor is visible on the window.
		*/
		inline bool isMouseVisible() const {
			return mouseVisible_;
		}
		/*
		* @param bool visible: Whether the mouse cursor is visible on the window.
		*/
		void setMouseVisible(bool);
		/*
		* @return Whether the mouse has moved on the window since the last update.
		*/
		inline bool isMouseMoved() const {
			return mousePos_.first != mousePos_.second;
		}
		/*
		* @return The current poition of the mouse cursor on the window.
		*/
		inline const glm::vec2& getMousePos() const {
			return mousePos_.first;
		}
		/*
		* Get the mouse's position in the 2D coordinate set of a camera.
		* @param const Camera& cam: The camera to project the mouse position through.
		* @return The current mouse position in the cam's 2D world.
		*/
		glm::vec2 getMousePos(const Camera&) const;
		/*
		* Determine whether a mouse button is currently down.
		* @param int btn: The button ID to test.
		* @return Whether the btn is currently down.
		*/
		bool isMouseBtnDown(int) const;
		/*
		* Determine whether a mouse button has just been pressed.
		* @param int btn: The button ID to test.
		* @return Whether the button has been pressed.
		*/
		inline bool isMouseBtnPressed(int btn) const {
			return isMouseBtnDown(btn) && !wasMouseBtnDown(btn);
		}
		/*
		* Determine whether a mouse button has just been released.
		* @param int btn: The button ID to test.
		* @return Whether the button has been released.
		*/
		inline bool isMouseBtnReleased(int btn) const {
			return !isMouseBtnDown(btn) && wasMouseBtnDown(btn);
		}
		/*
		* @return Whether the mouse scroll wheel has moved since the last update.
		*/
		inline bool isMouseScrolled() const {
			return (mouseScroll_.first != mouseScroll_.second)
				&& (mouseScroll_.first != glm::vec2(0.0f, 0.0f));
		}
		/*
		* @return The current scroll distance of the mouse for this update.
		*/
		inline const glm::vec2& getMouseScroll() const {
			return mouseScroll_.first;
		}
		/*
		* @return The set of IDs of all currently connected game controllers.
		*/
		std::vector<int> getCtrlIDs() const;
		/*
		* Determine whether a game controller is connected.
		* @param int ctrl: The controller ID to test.
		* @return Whether the ctrl is connected.
		*/
		bool isCtrlConnected(int) const;
		/*
		* Determine whether a game controller button is currently down.
		* @param int ctrl: The controller ID to test.
		* @param int btn: The button ID to test.
		* @return Whether the controller button is down.
		*/
		bool isCtrlBtnDown(int, int) const;
		/*
		* Determine whether a game controller button has just been pressed.
		* @param int ctrl: The controller ID to test.
		* @param int btn: The button ID to test.
		* @return Whether the controller button has been pressed.
		*/
		inline bool isCtrlBtnPressed(int ctrl, int btn) const {
			return isCtrlBtnDown(ctrl, btn) && !wasCtrlBtnDown(ctrl, btn);
		}
		/*
		* Determine whether a game controller button has just been released.
		* @param int ctrl: The controller ID to test.
		* @param int btn: The button ID to test.
		* @return Whether the controller button has been released.
		*/
		inline bool isCtrlBtnReleased(int ctrl, int btn) const {
			return !isCtrlBtnDown(ctrl, btn) && wasCtrlBtnDown(ctrl, btn);
		}
		/*
		* Determine whether a game controller axis has moved since last update.
		* @param int ctrl: The controller ID to test.
		* @param int axis: The axis ID to test.
		* @return Whether the controller axis has moved.
		*/
		bool isCtrlAxisMoved(int, int) const;
		/*
		* Get the current position of a game controller axis.
		* @param int ctrl: The controller ID to test.
		* @param int axis: The axis ID to test.
		* @return The current controller axis position.
		*/
		float getCtrlAxisPos(int, int) const;

	private:
		// The set of input listeners currently receiving callbacks.
		std::vector<InputLstr*> lstrs_;
		// Set of key IDs mapped to their current and previous states.
		std::map<int, std::pair<bool, bool>> keys_;
		// Whether the mouse cursor is enabled on the window.
		bool mouseEnabled_ = true;
		// Whether the mouse cursor is visible on the window.
		bool mouseVisible_ = true;
		// Current and previous position of the mouse cursor on the window.
		std::pair<glm::vec2, glm::vec2> mousePos_;
		// Set of mouse button IDs mapped to their current and previous states.
		std::map<int, std::pair<bool, bool>> mouseBtns_;
		// Current and previous position of the mouse scroll wheel.
		std::pair<glm::vec2, glm::vec2> mouseScroll_;
		// Set of game controller IDs to their current and previous states.
		std::map<int, std::pair<GLFWgamepadstate, GLFWgamepadstate>> ctrls_;
		// Set of game controllers to remove from memory on the next update.
		TSQueue<int> removedCtrls_;

		/*
		* Determine whether a keyboard key was down in the last update.
		* @param int key: The key ID to test.
		* @return Whether the key was down.
		*/
		bool wasKeyDown(int) const;
		/*
		* Determine whether a mouse button was down in the last update.
		* @param int btn: The button ID to test.
		* @return Whether the button was down.
		*/
		bool wasMouseBtnDown(int) const;
		/*
		* Determine whether a game controller button was down in the last update.
		* @param int ctrl: The controller ID to test.
		* @param int btn: The button ID to test.
		* @return Whether the button was down.
		*/
		bool wasCtrlBtnDown(int, int) const;
		/*
		* Set a keyboard key's current state to pressed.
		* @param int key: The key to press.
		*/
		void pressKey(int);
		/*
		* Set a keyboard key's current state to released.
		* @param int key: The key to release.
		*/
		void releaseKey(int);
		/*
		* Notify input listeners of a character typed on the keyboard.
		* @param char c: The character typed.
		*/
		void typeChar(char);
		/*
		* Change the mouse cursor's current position and notify input listeners.
		* @param const glm::vec2& mp: The new position of the mouse.
		*/
		void moveMouse(const glm::vec2&);
		/*
		* Set a mouse button's current state to pressed.
		* @param int btn: The button to press.
		*/
		void pressMouseBtn(int);
		/*
		* Set a mouse button's current state to released.
		* @param int btn: The button to release.
		*/
		void releaseMouseBtn(int);
		/*
		* Change the mouse scroll wheel's current position and notify input listeners.
		* @param const glm::vec2& scroll: The new scroll position.
		*/
		void scrollMouse(const glm::vec2&);
		/*
		* Connect a game controller.
		* @param int ctrl: The controller ID.
		*/
		void connectCtrl(int);
		/*
		* Disconnect a game controller.
		* @param int ctrl: The controller ID.
		*/
		void disconnectCtrl(int);
		/*
		* GLFW callback for keyboard key events.
		* @param GLFWwindow* window: The GLFW window handle to listen on.
		* @param int key: The key ID.
		* @param int scancode: The scancode of the keyboard key.
		* @param int action: The key's new state.
		* @param int mods: Modifier bits on the key event.
		*/
		static void KeyEvent(GLFWwindow*, int, int, int, int);
		/*
		* GLFW callback for keyboard typing events.
		* @param GLFWwindow* window: The GLFW window handle to listen on.
		* @param unsigned int codepoint: The ASCII character typed.
		*/
		static void CharEvent(GLFWwindow*, unsigned int);
		/*
		* GLFW callback for mouse cursor movement events.
		* @param GLFWwindow* window: The GLFW window handle to listen on.
		* @param double x: The new x-coordinate of the mouse on the window.
		* @param double y: The new y-coordinate of the mouse on the window.
		*/
		static void MousePosEvent(GLFWwindow*, double, double);
		/*
		* GLFW callback for mouse button events.
		* @param GLFWwindow* window: The GLFW window handle to listen on.
		* @param int button: The button ID.
		* @param int action: The new state of the button.
		* @param int mods: Modifier bits on the button event.
		*/
		static void MouseBtnEvent(GLFWwindow*, int, int, int);
		/*
		* GLFW callback for mouse scroll events.
		* @param GLFWwindow* window: The GLFW window handle to listen on.
		* @param double x: The scroll wheel's x distance.
		* @param double y: The scroll wheel's y distance.
		*/
		static void MouseScrollEvent(GLFWwindow*, double, double);
		/*
		* GLFW callback for game controller connection events.
		* @param int jid: The GLFW joystick ID for the controller.
		* @param int event: The type of controller event, connect / disconnect.
		*/
		static void CtrlEvent(int, int);
	};
}

#endif