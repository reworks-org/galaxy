///
/// AudioEngine.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include "galaxy/audio/AudioFileFactory.hpp"

#include "AudioEngine.hpp"

namespace galaxy
{
	namespace audio
	{
		AudioEngine::AudioEngine()
			: m_sfx_engine {nullptr}
			, m_music_engine {nullptr}
			, m_voice_engine {nullptr}
		{
			constexpr const auto options = irrklang::E_SOUND_ENGINE_OPTIONS::ESEO_MULTI_THREADED | irrklang::E_SOUND_ENGINE_OPTIONS::ESEO_USE_3D_BUFFERS |
										   irrklang::E_SOUND_ENGINE_OPTIONS::ESEO_MUTE_IF_NOT_FOCUSED;

			m_sfx_engine   = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, options);
			m_music_engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, options);
			m_voice_engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, options);

			if (!m_sfx_engine)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load irrKlang sfx engine.");
			}
			else
			{
				auto* factory = new AudioFileFactory();

				try
				{
					m_sfx_engine->addFileFactory(factory);
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				}

				factory->drop();
			}

			if (!m_music_engine)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load irrKlang music engine.");
			}
			else
			{
				auto* factory = new AudioFileFactory();

				try
				{
					m_music_engine->addFileFactory(factory);
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				}

				factory->drop();
			}

			if (!m_voice_engine)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load irrKlang voice engine.");
			}
			else
			{
				auto* factory = new AudioFileFactory();

				try
				{
					m_voice_engine->addFileFactory(factory);
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				}

				factory->drop();
			}
		}

		AudioEngine::~AudioEngine() noexcept
		{
			if (m_sfx_engine != nullptr)
			{
				m_sfx_engine->stopAllSounds();
				m_sfx_engine->drop();
				m_sfx_engine = nullptr;
			}

			if (m_music_engine != nullptr)
			{
				m_music_engine->stopAllSounds();
				m_music_engine->drop();
				m_music_engine = nullptr;
			}

			if (m_voice_engine != nullptr)
			{
				m_voice_engine->stopAllSounds();
				m_voice_engine->drop();
				m_voice_engine = nullptr;
			}
		}

		std::shared_ptr<Audio> AudioEngine::make_sfx(const std::string& filename, const float volume) noexcept
		{
			auto* source = m_sfx_engine->addSoundSourceFromFile(filename.c_str(), irrklang::ESM_AUTO_DETECT, true);
			source->setDefaultVolume(std::clamp(volume, 0.0f, 1.0f));

			auto audio = std::make_shared<Audio>();
			audio->set_data(source, m_sfx_engine);

			return audio;
		}

		std::shared_ptr<Audio> AudioEngine::make_music(const std::string& filename, const float volume) noexcept
		{
			auto* source = m_music_engine->addSoundSourceFromFile(filename.c_str(), irrklang::ESM_AUTO_DETECT, true);
			source->setDefaultVolume(std::clamp(volume, 0.0f, 1.0f));

			auto audio = std::make_shared<Audio>();
			audio->set_data(source, m_music_engine);

			return audio;
		}

		std::shared_ptr<Audio> AudioEngine::make_voice(const std::string& filename, const float volume) noexcept
		{
			auto* source = m_voice_engine->addSoundSourceFromFile(filename.c_str(), irrklang::ESM_AUTO_DETECT, true);
			source->setDefaultVolume(std::clamp(volume, 0.0f, 1.0f));

			auto audio = std::make_shared<Audio>();
			audio->set_data(source, m_voice_engine);

			return audio;
		}

		void AudioEngine::toggle_pause_all(const bool paused) noexcept
		{
			m_sfx_engine->setAllSoundsPaused(paused);
			m_music_engine->setAllSoundsPaused(paused);
			m_voice_engine->setAllSoundsPaused(paused);
		}

		void AudioEngine::stop_all() noexcept
		{
			m_sfx_engine->stopAllSounds();
			m_music_engine->stopAllSounds();
			m_voice_engine->stopAllSounds();
		}

		void AudioEngine::set_sfx_volume(const float volume) noexcept
		{
			m_sfx_engine->setSoundVolume(std::clamp(volume, 0.0f, 1.0f));
		}

		void AudioEngine::set_music_volume(const float volume) noexcept
		{
			m_music_engine->setSoundVolume(std::clamp(volume, 0.0f, 1.0f));
		}

		void AudioEngine::set_voice_volume(const float volume) noexcept
		{
			m_voice_engine->setSoundVolume(std::clamp(volume, 0.0f, 1.0f));
		}
	} // namespace audio
} // namespace galaxy