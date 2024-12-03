///
/// Sound.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_SOUND_HPP_
#define GALAXY_AUDIO_SOUND_HPP_

#include <raylib.hpp>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
GALAXY_DISABLE_WARNING(26437)
#endif

namespace galaxy
{
	namespace audio
	{
		///
		/// Small audio file loaded completely into memory.
		///
		class Sound : public ::Sound
		{
		public:
			///
			/// Constructor.
			///
			Sound() noexcept;

			///
			/// Move constructor.
			///
			Sound(Sound&&) noexcept;

			///
			/// Move assignment operator.
			///
			Sound& operator=(Sound&&) noexcept;

			///
			/// Destructor.
			///
			~Sound() noexcept;

			///
			/// Load sound from file.
			///
			/// \param file_name The name of the file to load the sound from.
			///
			void load(const std::string& file_name);

			///
			/// Unload sound data.
			///
			void unload() noexcept;

			///
			/// Check if a sound is valid (data loaded and buffers initialized).
			///
			/// \return True if the sound is valid, false otherwise.
			///
			[[nodiscard]]
			bool valid() const noexcept;

			///
			/// Update sound buffer with new data.
			///
			/// \param data The new data for the sound buffer.
			/// \param sample_count The number of samples in the new data.
			///
			void update(const void* data, const int sample_count) noexcept;

			///
			/// Play a sound.
			///
			void play() noexcept;

			///
			/// Stop playing a sound.
			///
			void stop() noexcept;

			///
			/// Pause a sound.
			///
			void pause() noexcept;

			///
			/// Resume a paused sound.
			///
			void resume() noexcept;

			///
			/// Check if a sound is currently playing.
			///
			/// \return True if the sound is playing, false otherwise.
			///
			[[nodiscard]]
			bool is_playing() const noexcept;

			///
			/// Set volume for a sound (1.0 is max level).
			///
			/// \param volume The volume level to set.
			///
			void set_volume(const float volume) noexcept;

			///
			/// Set pitch for a sound (1.0 is base level).
			///
			/// \param pitch The pitch level to set.
			///
			void set_pitch(const float pitch) noexcept;

			///
			/// Set pan for a sound (0.5 is center).
			///
			/// \param pan The pan level to set.
			///
			void set_pan(const float pan) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Sound(const Sound&) = delete;

			///
			/// Copy assignment operator.
			///
			Sound& operator=(const Sound&) = delete;
		};
	} // namespace audio
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
