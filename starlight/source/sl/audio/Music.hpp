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

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;
typedef struct ALLEGRO_SAMPLE_INSTANCE ALLEGRO_SAMPLE_INSTANCE;

namespace sl
{
	///
	/// Representation of a piece of music.
	/// Well, a wrapper around ALLEGRO_SAMPLE for music, anyway.
	///
	class Music final
	{
		friend class cereal::access;
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///			     music file path does not need to contain folder path, done for you based off of engine config.
		///
		explicit Music(const sol::table& table);

		///
		/// Destructor.
		///
		~Music();
		
		///
		/// Play the song from the beginning.
		///
		void play();

		///
		/// Stops the current song.
		///
		void stop();

		///
		/// Resumes the currently stopped song from the position where it stopped.
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
		///
		/// Default constructor.
		/// Deleted.
		///
		Music() = delete;

		///
		/// Cereal serialize function.
		///	
		template<class Archive>
		inline void serialize(Archive& ar)
		{
			stop();
			ar(m_position);
			resume();
		}

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