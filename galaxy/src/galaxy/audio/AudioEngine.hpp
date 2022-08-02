///
/// AudioEngine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_AUDIOENGINE_HPP_
#define GALAXY_AUDIO_AUDIOENGINE_HPP_

#include "galaxy/audio/Audio.hpp"

namespace galaxy
{
	namespace audio
	{
		class AudioEngine final
		{
		public:
			///
			/// Constructor.
			///
			AudioEngine();

			///
			/// Destructor.
			///
			~AudioEngine() noexcept;

			///
			/// Load a new sound effect into the engine.
			///
			/// \param filename File to load from VFS.
			/// \param volume Sound volume. 0.0f - 1.0f.
			///
			/// \return Shared pointer to object.
			///
			[[nodiscard]] std::shared_ptr<Audio> make_sfx(const std::string& filename, const float volume) noexcept;

			///
			/// Load new music into the engine.
			///
			/// \param filename File to load from VFS.
			/// \param volume Sound volume. 0.0f - 1.0f.
			///
			/// \return Shared pointer to object.
			///
			[[nodiscard]] std::shared_ptr<Audio> make_music(const std::string& filename, const float volume) noexcept;

			///
			/// Load a new voiceover into the engine.
			///
			/// \param filename File to load from VFS.
			/// \param volume Sound volume. 0.0f - 1.0f.
			///
			/// \return Shared pointer to object.
			///
			[[nodiscard]] std::shared_ptr<Audio> make_voice(const std::string& filename, const float volume) noexcept;

			///
			/// Pauses or unpauses all currently playing sounds.
			///
			/// \param paused True to pause all, false to unpause all.
			///
			void toggle_pause_all(const bool paused) noexcept;

			///
			/// Stops all currently playing sounds.
			///
			void stop_all() noexcept;

			///
			/// \brief Sets master sound effect volume.
			///
			/// This value is multiplied with all sounds played.
			/// \param volume 0 (silent) to 1.0f (full volume).
			///
			void set_sfx_volume(const float volume) noexcept;

			///
			/// \brief Sets master music volume.
			///
			/// This value is multiplied with all sounds played.
			/// \param volume 0 (silent) to 1.0f (full volume).
			///
			void set_music_volume(const float volume) noexcept;

			///
			/// \brief Sets master voiceover volume.
			///
			/// This value is multiplied with all sounds played.
			/// \param volume 0 (silent) to 1.0f (full volume).
			///
			void set_voice_volume(const float volume) noexcept;

		private:
			///
			/// Copy constructor.
			///
			AudioEngine(const AudioEngine&) = delete;

			///
			/// Move constructor.
			///
			AudioEngine(AudioEngine&&) = delete;

			///
			/// Copy assignment operator.
			///
			AudioEngine& operator=(const AudioEngine&) = delete;

			///
			/// Move assignment operator.
			///
			AudioEngine& operator=(AudioEngine&&) = delete;

		private:
			///
			/// Audio engine for sound effects.
			///
			irrklang::ISoundEngine* m_sfx_engine;

			///
			/// Audio engine for music.
			///
			irrklang::ISoundEngine* m_music_engine;

			///
			/// Audio engine for voices.
			///
			irrklang::ISoundEngine* m_voice_engine;
		};
	} // namespace audio
} // namespace galaxy

#endif