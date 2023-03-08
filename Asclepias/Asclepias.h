/*
* File:		Asclepias.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_ASCLEPIAS_H
#define ASC_ASCLEPIAS_H

#include "Logging.h"
#include "Graphics.h"
#include "Windowing.h"
#include "Input.h"
#include "Audio.h"
#include "UI.h"

namespace ASC {
	// Abstract scene interface for implementing Asclepias applciations.
	class Scene : public InputLstr {
	protected:
		friend class App;
		friend class UIGroup;
		
		/*
		* Call init() and flag this scene as initialized.
		* @return Whether the scene is already initialized or could be.
		*/
		bool initScene();
		/*
		* Initialize this scene's memory.
		* @return Whether this scene could initialize.
		*/
		virtual bool init() = 0;
		/*
		* Enter this scene from another.
		* @param Scene& prev: The previous scene or this one if this is the
		* first scene.
		*/
		virtual void enter(Scene&) = 0;
		/*
		* Draw this scene's graphics.
		*/
		virtual void draw() = 0;
		/*
		* Process user input to this scene.
		* @return Whether user input is such that the App should keep running.
		*/
		virtual bool processInput() = 0;
		/*
		* A UI component has triggered an event on this scene.
		* @param unsigned int gID: The ID of the component's UI group.
		* @param unsigned int cID: The ID of the UI component.
		* @param unsigned int eID: The ID of the event type.
		*/
		virtual void cmptEvent(unsigned int, unsigned int, unsigned int) = 0;
		/*
		* Update this scene's logic and physics.
		* @param float dt: Timestep since the last update.
		*/
		virtual void update(float) = 0;
		/*
		* Leave this scene for another.
		* @param Scene& next: The next scene for the app or this scene if the
		* app is terminating.
		*/
		virtual void leave(Scene&) = 0;
		/*
		* Free this scene's memory.
		*/
		virtual void destroy() = 0;
		/*
		* Call destroy() and flag this scene as uninitialized.
		*/
		void destroyScene();

	private:
		// Whether this scene's memory is initialized.
		bool initialized_ = false;
	};

	// Entry point singleton for the Asclepias framework.
	class App {
	public:
		// Configuration structure for Apps.
		struct Config {
			// Configuration structure for App's log manager.
			struct Log {
				// Whether this log should print to the console.
				bool console
#ifdef _DEBUG
					= true;
#else
					= false;
#endif
				// The file paths for this log to print messages to.
				std::vector<std::string> fileNames = { "ASC.log" };
				// The format for this log to print timestamps in.
				std::string timestampFmt = "%Y.%m.%d.%H%M.%S";
			} log;
			// Configuration structure for App's window manager.
			struct Window {
				// The dimensions for this window.
				glm::ivec2 dims = glm::ivec2(800, 600);
				// The title for this window.
				std::string title = "Asclepias";
				// Whether this window should open in fullscreen mode.
				bool fullscreen = false;
			} window;
			// Configuration structure for App's audio manager.
			struct Audio {
				// The initial global volume for audio.
				float volume = 1.0f;
			} audio;
			// Configuration structure for App's renderer.
			struct Graphics {
				// The color to clear the window to.
				glm::vec3 clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
			} graphics;
			// Configuration structure for App's update loop timing.
			struct Physics {
				// Targeted number of updates per second.
				double targetUPS = 60.0;
				// Maximum numbers of update() calls per frame.
				unsigned int maxUPF = 10;
			} physics;
			// The initial scene for this app.
			Scene& startScene;

			/*
			* Construct an App configuration structure.
			* @param Scene& startScene: The initial scene for this app.
			*/
			Config(Scene&);
		};
		/*
		* Initialize the Asclepias framework.
		* @param const Config& conf: The configuration structure for this app.
		*/
		static bool Init(const Config&);
		/*
		* Change the scene targeted by this app.
		* @param Scene& scene: The new scene for this app to target.
		*/
		static void SetScene(Scene&);
		/*
		* Run this app's update loop.
		*/
		static void Run();
		/*
		* Free this app's memory and utilities.
		*/
		static void Destroy();
		/*
		* @return This app's log manager.
		*/
		inline static LogMgr& log() {
			return inst_->log_;
		}
		/*
		* @return This app's window manager.
		*/
		inline static WindowMgr& window() {
			return inst_->window_;
		}
		/*
		* @return This app's user input manager.
		*/
		inline static InputMgr& input() {
			return inst_->input_;
		}
		/*
		* @return This app's audio manager.
		*/
		inline static AudioMgr& audio() {
			return inst_->audio_;
		}
		/*
		* @return This app's renderer.
		*/
		inline static Renderer& renderer() {
			return inst_->renderer_;
		}

	private:
		// The singleton instance of this app.
		static App* inst_;
		// This app's log manager.
		LogMgr log_;
		// This app's window manager.
		WindowMgr window_;
		// This app's input manager.
		InputMgr input_;
		// This app's audio manager.
		AudioMgr audio_;
		// This app's renderer.
		Renderer renderer_;
		// Targeted updates per second for app.
		double targetUPS_ = 0.0;
		// Maximum number of updates per frame for app.
		unsigned int maxUPF_ = 0;
		// The set of all scenes targeted by this app for its runtime.
		std::vector<Scene*> scenes_;
		// The currently targeted scene by this app.
		Scene* scene_ = 0;

		/*
		* Construct an App instance.
		*/
		App() {}
		/*
		* Do not use the App's copy constructor.
		*/
		App(const App&) = delete;
		/*
		* Do not use the App's assignment operator.
		*/
		App& operator = (const App&) = delete;
	};
}

#endif