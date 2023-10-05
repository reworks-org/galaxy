///
/// AudioEngine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_AUDIOENGINE_HPP_
#define GALAXY_MEDIA_AUDIOENGINE_HPP_

#include <miniaudio.h>

#include "galaxy/resource/Media.hpp"
#include "galaxy/utils/Globals.hpp"
#include "galaxy/utils/StringUtils.hpp"

namespace galaxy
{
	namespace media
	{
		///
		/// MiniAudio high level API engine.
		///
		/// \tparam type Enum type of sound for this engine.
		///
		template<SoundType type>
		class AudioEngine final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param listener_count Amount of listeners to spacialize audio around.
			///
			AudioEngine(const int listener_count);

			///
			/// Destructor.
			///
			~AudioEngine();

			///
			/// \brief Start device.
			///
			/// Device is started for you.
			///
			void start_device();

			///
			/// \brief Stop device.
			///
			/// Device is stopped in destructor.
			///
			void stop_device();

			///
			/// Stop all playing audio.
			///
			void stop();

			///
			/// \brief Set a listeners position.
			///
			/// Coordinate system is where positive X points right, positive Y points up and negative Z points forward.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_position(const unsigned int id, const float x, const float y, const float z);

			///
			/// Set the velocity of a sound used for doppler effect.
			///
			/// \param id Listener index / id.
			/// \param x X velocity.
			/// \param y Y velocity.
			/// \param z Z velocity.
			///
			void set_listener_velocity(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Set a listeners direction.
			///
			/// Direction is a forward vector.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_direction(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Set listener world up vector.
			///
			/// World up is an up vector.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_world_up(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Directional attenuation.
			///
			/// The listener can have a cone the controls how sound is attenuated based on the listener's direction.
			/// When a sound is between the inner and outer cones, it will be attenuated between 1 and the cone's outer gain.
			/// When a sound is inside the inner code, no directional attenuation is applied. When the sound is in between the inner and outer cones, the
			/// attenuation will be interpolated between 1 and the outer gain.
			///
			/// \param id Listener index / id.
			/// \param inner_angle In radians.
			/// \param outer_angle In radians.
			/// \param outer_gain Value attenuation will be set to when the sound is outside of the outer cone.
			///
			void set_listener_cone(const unsigned int id, const float inner_angle, const float outer_angle, const float outer_gain);

			///
			/// Sets master volume.
			///
			/// \param volume Scales linearly. 0 is muted. 1 is default. 1+ is gain.
			///
			void set_volume(const float volume);

			///
			/// Toggle a listener.
			///
			/// \param id Listener index / id.
			/// \param enable True to enable, false to disable.
			///
			void toggle_listener(const unsigned int id, const bool enable);

			///
			/// Check if a listener is enabled.
			///
			/// \param id Listener index / id.
			///
			[[nodiscard]] bool is_listener_enabled(const unsigned int id);

			///
			/// \brief Gets the nearest listener.
			///
			/// Coordinate system is where positive X points right, positive Y points up and negative Z points forward.
			///
			/// \param id Listener index / id.
			/// \param x Absolute Pos X.
			/// \param y Absolute Pos Y.
			/// \param z Absolute Pos Z.
			///
			/// \return Listener index / id.
			///
			[[nodiscard]] unsigned int find_closest_listener(const float x, const float y, const float z);

			///
			/// Get the total number of listeners.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int get_listener_count() const;

			///
			/// Get sample rate of engine.
			///
			/// \return Unsigned int. Most devices default to 48000.
			///
			[[nodiscard]] std::uint32_t get_samplerate() const;

			///
			/// Get number of channels audio is using.
			///
			/// \return Unsigned int. Most devices default to 2.
			///
			[[nodiscard]] std::uint32_t get_channels() const;

			///
			/// Get internal engine.
			///
			/// \return Pointer to ma_engine structure.
			///
			[[nodiscard]] ma_engine* get_engine();

		private:
			///
			/// Constructor.
			///
			AudioEngine() = delete;

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
			/// Handles sound effects.
			///
			ma_engine m_engine;

			///
			/// Log structure.
			///
			ma_log m_log;

			///
			/// Callback for when logging is done.
			///
			ma_log_callback m_callback;
		};

		///
		/// Engine for playing sound effects.
		///
		using SoundEngine = media::AudioEngine<SoundType::SFX>;

		///
		/// Engine for playing streaming audio like music.
		///
		using MusicEngine = media::AudioEngine<SoundType::MUSIC>;

		///
		/// Separate streaming engine for dialogue, to control easier.
		///
		using DialogueEngine = media::AudioEngine<SoundType::DIALOGUE>;

		template<SoundType type>
		inline AudioEngine<type>::AudioEngine(const int listener_count)
			: m_engine {}
			, m_log {}
			, m_callback {}
		{
			if (ma_log_init(nullptr, &m_log) == MA_SUCCESS)
			{
				m_callback.onLog = [](void* user_data, ma_uint32 level, const char* message) -> void {
					GALAXY_UNUSED(user_data);

					auto msg = std::string(message);
					strutils::replace_all(msg, "\t", " ");
					strutils::replace_all(msg, "\n", " ");
					strutils::trim(msg);
					strutils::make_single_spaced(msg);

					switch (level)
					{
						case MA_LOG_LEVEL_INFO:
							GALAXY_LOG(GALAXY_INFO, "{0}", msg);
							break;

						case MA_LOG_LEVEL_WARNING:
							GALAXY_LOG(GALAXY_WARNING, "{0}", msg);
							break;

						case MA_LOG_LEVEL_ERROR:
							GALAXY_LOG(GALAXY_ERROR, "{0}", msg);
							break;

						default:
							GALAXY_LOG(GALAXY_DEBUG, "{0}", msg);
							break;
					}
				};

				m_callback.pUserData = nullptr;

				if (ma_log_register_callback(&m_log, m_callback) == MA_SUCCESS)
				{
					ma_engine_config config = ma_engine_config_init();
					config.pLog             = &m_log;
					config.listenerCount    = std::clamp(listener_count, 1, MA_ENGINE_MAX_LISTENERS);
					config.channels         = 0;
					config.noAutoStart      = true;
					config.sampleRate       = 0;

					if (ma_engine_init(&config, &m_engine) == MA_SUCCESS)
					{
						start_device();
					}
					else
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize a miniaudio engine.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to set miniaudio logging callback.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize miniaudio logging.");
			}
		}

		template<SoundType type>
		inline AudioEngine<type>::~AudioEngine()
		{
			stop_device();
			ma_engine_uninit(&m_engine);
			ma_log_unregister_callback(&m_log, m_callback);
			ma_log_uninit(&m_log);
		}

		template<SoundType type>
		inline void AudioEngine<type>::start_device()
		{
			ma_engine_start(&m_engine);
		}

		template<SoundType type>
		inline void AudioEngine<type>::stop_device()
		{
			ma_engine_stop(&m_engine);
		}

		template<SoundType type>
		inline void AudioEngine<type>::stop()
		{
			if constexpr (type == SoundType::SFX)
			{
				auto& ae = core::ServiceLocator<resource::SFXCache>::ref();
				for (const auto& [key, value] : ae.cache())
				{
					value->stop();
				}
			}
			else if constexpr (type == SoundType::MUSIC)
			{
				auto& ae = core::ServiceLocator<resource::MusicCache>::ref();
				for (const auto& [key, value] : ae.cache())
				{
					value->stop();
				}
			}
			else if constexpr (type == SoundType::DIALOGUE)
			{
				auto& ae = core::ServiceLocator<resource::DialogueCache>::ref();
				for (const auto& [key, value] : ae.cache())
				{
					value->stop();
				}
			}
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_listener_position(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_position(&m_engine, id, x, y, z);
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_listener_velocity(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_velocity(&m_engine, id, x, y, z);
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_listener_direction(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_direction(&m_engine, id, x, y, z);
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_listener_world_up(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_world_up(&m_engine, id, x, y, z);
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_listener_cone(const unsigned int id, const float inner_angle, const float outer_angle, const float outer_gain)
		{
			ma_engine_listener_set_cone(&m_engine, id, inner_angle, outer_angle, outer_gain);
		}

		template<SoundType type>
		inline void AudioEngine<type>::set_volume(const float volume)
		{
			ma_engine_set_volume(&m_engine, volume);
		}

		template<SoundType type>
		inline void AudioEngine<type>::toggle_listener(const unsigned int id, const bool enable)
		{
			ma_engine_listener_set_enabled(&m_engine, id, enable);
		}

		template<SoundType type>
		inline bool AudioEngine<type>::is_listener_enabled(const unsigned int id)
		{
			return ma_engine_listener_is_enabled(&m_engine, id);
		}

		template<SoundType type>
		inline unsigned int AudioEngine<type>::find_closest_listener(const float x, const float y, const float z)
		{
			return ma_engine_find_closest_listener(&m_engine, x, y, z);
		}

		template<SoundType type>
		inline unsigned int AudioEngine<type>::get_listener_count() const
		{
			return ma_engine_get_listener_count(&m_engine);
		}

		template<SoundType type>
		inline std::uint32_t AudioEngine<type>::get_samplerate() const
		{
			return ma_engine_get_sample_rate(&m_engine);
		}

		template<SoundType type>
		inline std::uint32_t AudioEngine<type>::get_channels() const
		{
			return ma_engine_get_channels(&m_engine);
		}

		template<SoundType type>
		inline ma_engine* AudioEngine<type>::get_engine()
		{
			return &m_engine;
		}
	} // namespace media
} // namespace galaxy

#endif