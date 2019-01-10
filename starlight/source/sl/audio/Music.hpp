///
/// Music.hpp
/// starlight
///
/// Created by reworks on 13/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_MUSIC_HPP_
#define STARLIGHT_MUSIC_HPP_

#include "sl/libs/sol2/sol_forward.hpp"

typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;
typedef struct ALLEGRO_SAMPLE_INSTANCE ALLEGRO_SAMPLE_INSTANCE;
typedef enum ALLEGRO_PLAYMODE ALLEGRO_PLAYMODE;

namespace sl
{
	///
	/// Representation of a piece of music.
	/// Well, a wrapper around ALLEGRO_SAMPLE for music, anyway.
	///
	class Music
	{
	public:
		///
		/// Constructor.
		///
		/// \param file Music file to load. Audio folder path from config is added for you. Do NOT append it here.
		/// \param playmode ALLEGRO playmode to use.
		/// \param pan -1.0f to 1.0f Left to right pan of audio.
		/// \param speed 0.1f minimum. Playback speed of audio. 0.5 is 50% speed for example.
		/// \param volume 0.0f minimum. 1.0f is normal. 2.0f is double, etc.
		///
		Music(const std::string& file, const ALLEGRO_PLAYMODE playmode, const int pan, const int speed, const int volume);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table containing data.
		///			     music file path does not need to contain folder path, done for you based off of engine config.
		///
		explicit Music(const sol::table& table);

		///
		/// Destructor.
		///
		~Music() noexcept;
		
		///
		/// Play the song from the beginning.
		///
		void play() const;

		///
		/// Stops the current song.
		///
		void stop();

		///
		/// Resumes the currently stopped song from the position where it stopped.
		///
		void resume() const;

		///
		/// Change the pan.
		///
		/// \param pan CONST 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		///
		/// \return True on success.
		///
		bool setPan(const float pan);

		///
		/// Change the speed.
		///
		/// \param speed CONST Relative speed at which the sample is played. 1.0 is normal. 0.5 is at 50% speed, etc. Minimum is 0.1f.
		///
		/// \return True on success.
		///
		bool setSpeed(const float speed);

		///
		/// Change the volume.
		///
		/// \param volume CONST Relative volume (gain) at which the sample is played. 1.0 is 100%. 0.5 is 50%, etc. Minimum of 0.0f.
		///
		/// \return True on success.
		///
		bool setVolume(const float volume);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Music() = delete;

	private:
		///
		/// Position of the song at the point its paused. Used to pause and resume the music.
		///
		unsigned int m_position;

		///
		/// Pointer to the music data.
		///
		ALLEGRO_SAMPLE* m_music;

		///
		/// An instance of the music, used to stream the music into memory.
		///
		ALLEGRO_SAMPLE_INSTANCE* m_instance;
	};
}

#endif