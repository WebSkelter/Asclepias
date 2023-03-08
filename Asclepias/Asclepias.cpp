/*
* File:		Asclepias.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define Scene functions.

	bool Scene::initScene() {
		if (initialized_) {
			return true;
		}
		initialized_ = init();
		return initialized_;
	}

	void Scene::destroyScene() {
		if (!initialized_) {
			return;
		}
		initialized_ = false;
		destroy();
	}

	// Define App functions.

	App::Config::Config(Scene& scene) : startScene(scene) {}

	App* App::inst_ = 0;

	bool App::Init(const App::Config& conf) {
		if (inst_ != 0) {
			return false;
		}
		inst_ = new App;

		inst_->log_.init(conf.log.console, conf.log.fileNames, conf.log.timestampFmt);
		ASCLOG(App, Info, "Initialized logging system.");

		if (!inst_->window_.init(conf.window.dims, conf.window.title, conf.window.fullscreen)) {
			ASCLOG(App, Error, "Failed to initialize windowing module.");
			return false;
		}
		ASCLOG(App, Info, "Initialized window.");

		inst_->input_.init();
		ASCLOG(App, Info, "Initialized input manager.");

		inst_->audio_.init(conf.audio.volume);
		ASCLOG(App, Info, "Initialized audio manager.");

		if (!inst_->renderer_.init(conf.graphics.clearColor)) {
			ASCLOG(App, Error, "Failed to initialize renderer.");
			return false;
		}
		ASCLOG(App, Info, "Initialized OpenGL renderer.");

		inst_->targetUPS_ = conf.physics.targetUPS;
		inst_->maxUPF_ = conf.physics.maxUPF;

		inst_->scene_ = &conf.startScene;
		return true;
	}

	void App::SetScene(Scene& scene) {
		if (std::find(inst_->scenes_.begin(), inst_->scenes_.end(), &scene)
			== inst_->scenes_.end()) {
			inst_->scenes_.push_back(&scene);
		}
		if (inst_->scene_ != 0 && inst_->scene_ != &scene) {
			inst_->input_.removeLstr(*inst_->scene_);
			inst_->scene_->leave(scene);
		}
		if (scene.initScene()) {
			if (inst_->scene_ != 0) {
				scene.enter(*inst_->scene_);
			}
			else {
				scene.enter(scene);
			}
			inst_->input_.addLstr(scene);
			inst_->scene_ = &scene;
		}
		ASCLOG(App, Info, "Set to scene at ", &scene, ".");
	}

	void App::Run() {
		SetScene(*inst_->scene_);

		double start = glfwGetTime();
		double now = glfwGetTime();
		double elapsed = 0.0;
		double dt = 0.0;
		unsigned int updates = 0;
		while (inst_->window_.update()) {
			inst_->renderer_.begin();
			inst_->scene_->draw();
			inst_->renderer_.end();

			inst_->input_.update();
			if (!inst_->scene_->processInput()) {
				break;
			}

			now = glfwGetTime();
			elapsed = now - start;
			start = now;
			dt = elapsed * inst_->targetUPS_;
			while (dt > 1.0 && updates < inst_->maxUPF_) {
				inst_->scene_->update(1.0f);
				inst_->renderer_.update(1.0f);
				dt -= 1.0;
				updates++;
			}
			inst_->scene_->update((float)dt);
			inst_->renderer_.update((float)dt);
			updates = 0;

			Sleep(1);
		}
	}

	void App::Destroy() {
		inst_->scene_->leave(*inst_->scene_);
		for (Scene* scene : inst_->scenes_) {
			scene->destroyScene();
		}
		inst_->scenes_.clear();
		inst_->scene_ = 0;

		ASCLOG(App, Info, "Destroying renderer.");
		inst_->renderer_.destroy();

		ASCLOG(App, Info, "Destroying audio manager.");
		inst_->audio_.destroy();

		ASCLOG(App, Info, "Destroying input manager.");
		inst_->input_.destroy();

		ASCLOG(App, Info, "Destroying window.");
		inst_->window_.destroy();

		ASCLOG(App, Info, "Destroying logging system.");
		inst_->log_.destroy();

		delete inst_;
	}
}