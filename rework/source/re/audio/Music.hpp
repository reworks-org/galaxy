///
///  Music.hpp
///  rework
///
///  Created by reworks on 13/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_MUSIC_HPP_
#define REWORK_MUSIC_HPP_

#include "sol2/sol_forward.hpp"

typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;
typedef struct ALLEGRO_SAMPLE_INSTANCE ALLEGRO_SAMPLE_INSTANCE;

namespace re
{
	class Music
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Music(const sol::table& table);

		///
		/// Destructor.
		///
		~Music();

		///
		/// Play the song from the beginning.
		///
		void play();

		///
		/// Stops the song.
		///
		void stop();

		///
		/// Resumes a stopped song from the position where it stopped.
		///
		void resume();
		
		///
		/// Change the pan.
		///
		/// \param pan CONST 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		///
		void setPan(const float pan);

		///
		/// Change the speed.
		///
		/// \param speed CONST Relative speed at which the sample is played. 1.0 is normal. 0.5 is at 50% speed, etc.
		///
		void setSpeed(const float speed);

		///
		/// Change the volume.
		///
		/// \param volume CONST Relative volume (gain) at which the sample is played. 1.0 is 100%. 0.5 is 50%, etc.
		///
		void setVolume(const float volume);

	private:
		unsigned int m_position;

		ALLEGRO_SAMPLE* m_music;
		ALLEGRO_SAMPLE_INSTANCE* m_instance;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Music() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Music(const Music&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Music(Music&&) = delete;
	};
}

#endif