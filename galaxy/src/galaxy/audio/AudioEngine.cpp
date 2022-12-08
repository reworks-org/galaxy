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
	namespace audio
	{
		AudioEngine::AudioEngine(const int listener_count)
			: m_sfx_engine {}
			, m_music_engine {}
			, m_dialogue_engine {}
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

					result = ma_engine_init(&config, &m_sfx_engine);
					if (result != MA_SUCCESS)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize sfx engine.");
					}

					result = ma_engine_init(&config, &m_music_engine);
					if (result != MA_SUCCESS)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize music engine.");
					}

					result = ma_engine_init(&config, &m_dialogue_engine);
					if (result != MA_SUCCESS)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize dialogue engine.");
					}
				}
			}
		}

		AudioEngine::~AudioEngine() noexcept
		{
			ma_engine_uninit(&m_sfx_engine);
			ma_engine_uninit(&m_music_engine);
			ma_engine_uninit(&m_dialogue_engine);
			ma_log_unregister_callback(&m_log, m_callback);
			ma_log_uninit(&m_log);
		}

		void AudioEngine::stop() noexcept
		{
			for (auto& [key, ptr] : core::ServiceLocator<resource::Sounds>::ref().cache())
			{
				ptr->stop();
			}
		}

		void AudioEngine::set_listener_position(const unsigned int id, const float x, const float y, const float z) noexcept
		{
			ma_engine_listener_set_position(&m_sfx_engine, id, x, y, z);
			ma_engine_listener_set_position(&m_music_engine, id, x, y, z);
			ma_engine_listener_set_position(&m_dialogue_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_direction(const unsigned int id, const float x, const float y, const float z) noexcept
		{
			ma_engine_listener_set_direction(&m_sfx_engine, id, x, y, z);
			ma_engine_listener_set_direction(&m_music_engine, id, x, y, z);
			ma_engine_listener_set_direction(&m_dialogue_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_world_up(const unsigned int id, const float x, const float y, const float z) noexcept
		{
			ma_engine_listener_set_world_up(&m_sfx_engine, id, x, y, z);
			ma_engine_listener_set_world_up(&m_music_engine, id, x, y, z);
			ma_engine_listener_set_world_up(&m_dialogue_engine, id, x, y, z);
		}

		void AudioEngine::set_listener_cone(const unsigned int id, const float inner_angle, const float outer_angle, const float outer_gain) noexcept
		{
			ma_engine_listener_set_cone(&m_sfx_engine, id, inner_angle, outer_angle, outer_gain);
			ma_engine_listener_set_cone(&m_music_engine, id, inner_angle, outer_angle, outer_gain);
			ma_engine_listener_set_cone(&m_dialogue_engine, id, inner_angle, outer_angle, outer_gain);
		}

		void AudioEngine::toggle_listener(const unsigned int id, const bool enable) noexcept
		{
			ma_engine_listener_set_enabled(&m_sfx_engine, id, enable);
			ma_engine_listener_set_enabled(&m_music_engine, id, enable);
			ma_engine_listener_set_enabled(&m_dialogue_engine, id, enable);
		}

		void AudioEngine::set_sfx_volume(const float volume) noexcept
		{
			ma_engine_set_volume(&m_sfx_engine, volume);
		}

		void AudioEngine::set_music_volume(const float volume) noexcept
		{
			ma_engine_set_volume(&m_music_engine, volume);
		}

		void AudioEngine::set_dialogue_volume(const float volume) noexcept
		{
			ma_engine_set_volume(&m_dialogue_engine, volume);
		}

		ma_engine* AudioEngine::get_sfx_engine() noexcept
		{
			return &m_sfx_engine;
		}

		ma_engine* AudioEngine::get_music_engine() noexcept
		{
			return &m_music_engine;
		}

		ma_engine* AudioEngine::get_dialogue_engine() noexcept
		{
			return &m_dialogue_engine;
		}
	} // namespace audio
} // namespace galaxy