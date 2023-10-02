///
/// AudioEngine.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include "galaxy/utils/Globals.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "AudioEngine.hpp"

#undef ERROR

namespace galaxy
{
	namespace media
	{
		AudioEngine::AudioEngine(const int listener_count)
			: m_engine {}
			, m_log {}
			, m_callback {}
		{
			ma_result result = ma_log_init(nullptr, &m_log);
			if (result != MA_SUCCESS)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize miniaudio logging.");
			}
			else
			{
				m_callback.onLog = [](void* pUserData, ma_uint32 level, const char* pMessage) -> void {
					GALAXY_UNUSED(pUserData);

					auto msg = std::string(pMessage);
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

				result = ma_log_register_callback(&m_log, m_callback);
				if (result != MA_SUCCESS)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to set miniaudio logging callback.");
				}
				else
				{
					ma_engine_config config = ma_engine_config_init();
					config.pLog             = &m_log;
					config.listenerCount    = std::clamp(listener_count, 1, MA_ENGINE_MAX_LISTENERS);
					config.channels         = 0;
					config.noAutoStart      = true;
					config.sampleRate       = 0;

					result = ma_engine_init(&config, &m_engine);
					if (result != MA_SUCCESS)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize a miniaudio engine.");
					}
					else
					{
						start_device();
					}
				}
			}
		}

		AudioEngine::~AudioEngine()
		{
			stop_device();
			ma_engine_uninit(&m_engine);
			ma_log_unregister_callback(&m_log, m_callback);
			ma_log_uninit(&m_log);
		}

		void AudioEngine::start_device()
		{
			ma_engine_start(&m_engine);
		}

		void AudioEngine::stop_device()
		{
			ma_engine_stop(&m_engine);
		}

		void AudioEngine::stop()
		{
			for (auto& [key, ptr] : core::ServiceLocator<resource::Sounds>::ref().cache())
			{
				ptr->stop();
			}
		}

		void AudioEngine::set_listener_position(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_position(&m_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_velocity(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_velocity(&m_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_direction(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_direction(&m_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_world_up(const unsigned int id, const float x, const float y, const float z)
		{
			ma_engine_listener_set_world_up(&m_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_cone(const unsigned int id, const float inner_angle, const float outer_angle, const float outer_gain)
		{
			ma_engine_listener_set_cone(&m_engine, id, inner_angle, outer_angle, outer_gain);
		}

		void AudioEngine::set_volume(const float volume)
		{
			ma_engine_set_volume(&m_engine, volume);
		}

		void AudioEngine::toggle_listener(const unsigned int id, const bool enable)
		{
			ma_engine_listener_set_enabled(&m_engine, id, enable);
		}

		bool AudioEngine::is_listener_enabled(const unsigned int id)
		{
			return ma_engine_listener_is_enabled(&m_engine, id);
		}

		unsigned int AudioEngine::find_closest_listener(const float x, const float y, const float z)
		{
			return ma_engine_find_closest_listener(&m_engine, x, y, z);
		}

		unsigned int AudioEngine::get_listener_count() const
		{
			return ma_engine_get_listener_count(&m_engine);
		}

		std::uint32_t AudioEngine::get_samplerate() const
		{
			return ma_engine_get_sample_rate(&m_engine);
		}

		std::uint32_t AudioEngine::get_channels() const
		{
			return ma_engine_get_channels(&m_engine);
		}

		ma_engine* AudioEngine::get_engine()
		{
			return &m_engine;
		}
	} // namespace media
} // namespace galaxy