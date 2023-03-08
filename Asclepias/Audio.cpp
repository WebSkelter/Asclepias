/*
* File:		Audio.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define AudioMgr functions.
	
	bool AudioMgr::init(float volume) {
		if (audio_.init() != SoLoud::SO_NO_ERROR) {
			ASCLOG(Audio, Error, "Failed to initialize SoLoud library.");
			return false;
		}
		ASCLOG(Audio, Info, "Initialized SoLoud library.");
		setVolume(volume);
		return true;
	}

	bool AudioMgr::playEffect(const std::string& fileName) {
		if (effects_.find(fileName) == effects_.end()) {
			if (!loadEffect(fileName)) {
				return false;
			}
		}
		audio_.play(effects_.at(fileName));
		return true;
	}

	bool AudioMgr::playMusic(const std::string& fileName) {
		if (fileName.empty()) {
			if (music_ == 0) {
				return false;
			}
			if (!audio_.getPause(music_)) {
				return false;
			}
			audio_.setPause(music_, false);
			ASCLOG(Audio, Info, "Resumed music track ", fileName, ".");
		}
		else {
			if (effects_.find(fileName) == effects_.end()) {
				if (!loadEffect(fileName)) {
					return false;
				}
			}
			stopMusic();
			music_ = audio_.playBackground(effects_.at(fileName));
			audio_.setLooping(music_, true);
			ASCLOG(Audio, Info, "Playing music track ", fileName, ".");
		}
		musicPaused_ = false;
		return true;
	}

	bool AudioMgr::pauseMusic() {
		if (music_ == 0) {
			return false;
		}
		if (musicPaused_) {
			return false;
		}
		audio_.setPause(music_, true);
		musicPaused_ = true;
		ASCLOG(Audio, Info, "Paused music track.");
		return true;
	}

	void AudioMgr::stopMusic() {
		if (music_ != 0) {
			audio_.stop(music_);
		}
		music_ = 0;
		musicPaused_ = false;
		ASCLOG(Audio, Info, "Stopped music track.");
	}

	void AudioMgr::destroy() {
		effects_.clear();
		music_ = 0;
		musicPaused_ = false;
		audio_.stopAll();
		audio_.deinit();
		ASCLOG(Audio, Info, "Destroyed SoLoud library.");
	}

	void AudioMgr::setVolume(float volume) {
		if (volume < 0.0f) {
			volume = 0.0f;
		}
		if (volume > 1.0f) {
			volume = 1.0f;
		}
		audio_.setGlobalVolume(volume);
	}

	bool AudioMgr::loadEffect(const std::string& fileName) {
		ASCLOG(Audio, Info, "Loading sound effect ", fileName, ".");
		effects_[fileName] = SoLoud::Wav();
		if (effects_[fileName].load(fileName.c_str()) != SoLoud::SO_NO_ERROR) {
			ASCLOG(Audio, Warning, "Failed to load sound effect ", fileName, ".");
			return false;
		}
		return true;
	}
}