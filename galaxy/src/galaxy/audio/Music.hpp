///
/// Music.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_MUSIC_HPP_
#define GALAXY_AUDIO_MUSIC_HPP_

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
		/// Large audio file streamed.
		///
		class Music : public rl::Music
		{
		public:
			///
			/// Constructor.
			///
			Music() noexcept;

			///
			/// Move constructor.
			///
			Music(Music&&) noexcept;

			///
			/// Move assignment operator.
			///
			[[maybe_unused]]
			Music& operator=(Music&&) noexcept;

			///
			/// Destructor.
			///
			~Music() noexcept;

			///
			/// Load music from file.
			///
			/// \param file_name The name of the file to load the music from.
			///
			void load(const std::string& file_name);

			///
			/// Unload music data.
			///
			void unload() noexcept;

			///
			/// Check if music is valid (data loaded and buffers initialized).
			///
			/// \return True if the music is valid, false otherwise.
			///
			[[nodiscard]]
			bool valid() const noexcept;

			///
			/// Update music buffer.
			///
			void update() noexcept;

			///
			/// Play music.
			///
			void play() noexcept;

			///
			/// Stop playing music.
			///
			void stop() noexcept;

			///
			/// Pause music.
			///
			void pause() noexcept;

			///
			/// Resume a paused music.
			///
			void resume() noexcept;

			///
			/// Check if music is currently playing.
			///
			/// \return True if the music is playing, false otherwise.
			///
			[[nodiscard]]
			bool is_playing() const noexcept;

			///
			/// Seek music to a position (in seconds).
			///
			/// \param position The position to seek to (in seconds).
			///
			void seek(const float position) noexcept;

			///
			/// Set volume for music (1.0 is max level).
			///
			/// \param volume The volume level to set.
			///
			void set_volume(const float volume) noexcept;

			///
			/// Set pitch for music (1.0 is base level).
			///
			/// \param pitch The pitch level to set.
			///
			void set_pitch(const float pitch) noexcept;

			///
			/// Set pan for music (0.5 is center).
			///
			/// \param pan The pan level to set.
			///
			void set_pan(const float pan) noexcept;

			///
			/// Get music time length (in seconds).
			///
			/// \return The music time length (in seconds).
			///
			[[nodiscard]]
			float get_time_length() noexcept;

			///
			/// Get current music time played (in seconds).
			///
			/// \return The current music time played (in seconds).
			///
			[[nodiscard]]
			float get_time_played() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Music(const Music&) = delete;

			///
			/// Copy assignment operator.
			///
			Music& operator=(const Music&) = delete;
		};
	} // namespace audio
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
