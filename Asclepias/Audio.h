/*
* File:		Audio.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_AUDIO_H
#define ASC_AUDIO_H

#include <SoLoud/soloud.h>
#include <SoLoud/soloud_wav.h>
#include <map>
#include <string>

namespace ASC {
	// App utility for loading and playing sound effects and background music.
	class AudioMgr {
	public:
		/*
		* Initialize the SoLoud library and this audio manager's memory.
		* @param float volume: The initial global volume to set.
		* @return Whether the audio manager could be initialized.
		*/
		bool init(float);
		/*
		* Play a sound effect by file name.
		* @param const std::string& fileName: The file path to the WAV sound effect.
		* @return Whether the sound effect could be loaded/played.
		*/
		bool playEffect(const std::string&);
		/*
		* Play a new or the current music track.
		* @param const std::string& fileName: The file path to the WAV music, empty string
		* to resume current track.
		* @return Whether the music track could be played.
		*/
		bool playMusic(const std::string& = "");
		/*
		* Pause the current music track.
		* @return Whether the music track could be paused.
		*/
		bool pauseMusic();
		/*
		* @return Whether the music track is currently paused.
		*/
		inline bool isMusicPaused() {
			return musicPaused_;
		}
		/*
		* Stop the current music track.
		*/
		void stopMusic();
		/*
		* Stop the SoLoud library and free this audio manager's memory.
		*/
		void destroy();
		/*
		* @return The global volume of the SoLoud library.
		*/
		inline float getVolume() const {
			return audio_.getGlobalVolume();
		}
		/*
		* @param float volume: The global volume for the SoLoud library.
		*/
		void setVolume(float);

	private:
		// SoLoud library instance used to load and play WAV files.
		SoLoud::Soloud audio_;
		// Set of sound effects and music tracks loaded into memory.
		std::map<std::string, SoLoud::Wav> effects_;
		// The currently playing music track.
		SoLoud::handle music_ = 0;
		// Whether the music track is currently paused.
		bool musicPaused_ = false;

		/*
		* Load a WAV file via the SoLoud library.
		* @param const std::string& fileName: The path to the WAV file.
		* @return Whether the file could be loaded into memory.
		*/
		bool loadEffect(const std::string&);
	};
}

#endif