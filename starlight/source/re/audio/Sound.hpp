///
///  Sound.hpp
///  starlight
///
///  Created by reworks on 12/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SOUND_HPP_
#define REWORK_SOUND_HPP_

#include "sol2/sol_forward.hpp"

typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;

namespace sl
{
	class Sound final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Sound(const sol::table& table);
		
		///
		/// Destructor.
		///
		~Sound();

		///
		/// Play the sound.
		///
		void play();

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
		float m_pan;
		float m_speed;
		float m_volume;

		ALLEGRO_SAMPLE* m_sound;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Sound() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Sound(const Sound&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Sound(Sound&&) = delete;
	};
}

#endif