///
/// Audio.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_AUDIO_HPP_
#define GALAXY_AUDIO_AUDIO_HPP_

#include "irrKlang.h"

namespace galaxy
{
	namespace audio
	{
		class AudioEngine;

		///
		/// Class to help manage irrklang sounds and sources.
		///
		class Audio final
		{
			friend class AudioEngine;

		public:
			///
			/// Constructor.
			///
			Audio() noexcept;

			///
			/// Destructor.
			///
			~Audio() noexcept;

			///
			/// Plays a sound source as 2D sound.
			///
			/// \param loop Should the audio be looped?
			///
			void play(const bool loop) noexcept;

			///
			/// Stops all sounds.
			///
			void stop() noexcept;

			///
			/// Is audio currently playing?
			///
			/// \return Returns if a sound with the specified source is currently playing.
			///
			[[nodiscard]] bool is_playing() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Audio(const Audio&) = delete;

			///
			/// Move constructor.
			///
			Audio(Audio&&) = delete;

			///
			/// Copy assignment operator.
			///
			Audio& operator=(const Audio&) = delete;

			///
			/// Move assignment operator.
			///
			Audio& operator=(Audio&&) = delete;

			///
			/// Set pointers to use.
			///
			/// \param source Pointer to audio source.
			/// \param engine Pointer to audio engine.
			///
			void set_data(irrklang::ISoundSource* source, irrklang::ISoundEngine* engine) noexcept;

		private:
			///
			/// Pointer to source file to manage.
			///
			irrklang::ISoundSource* m_source;

			///
			/// Pointer to audio engine.
			///
			irrklang::ISoundEngine* m_engine;
		};
	} // namespace audio
} // namespace galaxy

#endif