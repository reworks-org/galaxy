///
/// Sound.hpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SOUND_HPP_
#define STARLIGHT_SOUND_HPP_

#include "sl/libs/sol2/sol_forward.hpp"

typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;

namespace sl
{
	///
	/// Representation of a sound effect.
	/// Well, a wrapper around ALLEGRO_SAMPLE for sfx, anyway.
	///
	class Sound
	{
	public:
		///
		/// Constructor.
		///
		/// \param file Sound file to load. Audio folder path from config is added for you. Do NOT append it here.
		/// \param pan -1.0f to 1.0f Left to right pan of audio.
		/// \param speed 0.1f minimum. Playback speed of audio. 0.5 is 50% speed for example.
		/// \param volume 0.0f minimum. 1.0f is normal. 2.0f is double, etc.
		///
		Sound(const std::string& file, const float pan, const float speed, const float volume);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table containing data.
		///			     sound file path does not need to contain folder path, done for you based off of engine config.
		///
		explicit Sound(const sol::table& table);
		
		///
		/// Destructor.
		///
		~Sound() noexcept;

		///
		/// Play the sound.
		///
		void play() const;

		///
		/// Change the pan.
		///
		/// \param pan CONST 0.0 is centred, -1.0f is left, 1.0f is right, or ALLEGRO_AUDIO_PAN_NONE.
		///
		void setPan(const float pan);

		///
		/// Change the speed.
		///
		/// \param speed CONST Relative speed at which the sample is played. 1.0 is normal. 0.5 is at 50% speed, etc. 0.1f minimum.
		///
		void setSpeed(const float speed);

		///
		/// Change the volume.
		///
		/// \param volume CONST Relative volume (gain) at which the sample is played. 1.0 is 100%. 0.5 is 50%, etc. 0.0f minimum.
		///
		void setVolume(const float volume);

	private:
		///
		/// Default constructor.
		/// Deleted. We don't want a sound to be constructed without data.
		///
		Sound() = delete;

	private:
		///
		/// The pan of the sample. Between -1.0f and 1.0f.
		///
		float m_pan;

		///
		/// Speed at which the sample is played. 0.1f minimum.
		///
		float m_speed;

		///
		/// Volume of the sample. 0.0f minimum.
		///
		float m_volume;

		///
		/// Pointer to the sound file in memory.
		///
		ALLEGRO_SAMPLE* m_sound;
	};
}

#endif