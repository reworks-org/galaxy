///
/// Audio.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Audio.hpp"

namespace galaxy
{
	namespace audio
	{
		Audio::Audio() noexcept
			: m_source {nullptr}
			, m_engine {nullptr}
		{
		}

		void Audio::set_data(irrklang::ISoundSource* source, irrklang::ISoundEngine* engine) noexcept
		{
			m_source = source;

			m_engine = engine;
			m_engine->grab();
		}

		Audio::Audio(const Audio& a) noexcept
		{
			this->m_source = a.m_source;
			this->m_engine = a.m_engine;

			this->m_engine->grab();
		}

		Audio::Audio(Audio&& a) noexcept
		{
			this->m_source = a.m_source;
			this->m_engine = a.m_engine;

			a.m_source = nullptr;
			a.m_engine = nullptr;
		}

		Audio& Audio::operator=(const Audio& a) noexcept
		{
			if (this != &a)
			{
				this->m_source = a.m_source;
				this->m_engine = a.m_engine;

				this->m_engine->grab();
			}

			return *this;
		}

		Audio& Audio::operator=(Audio&& a) noexcept
		{
			if (this != &a)
			{
				this->m_source = a.m_source;
				this->m_engine = a.m_engine;

				a.m_source = nullptr;
				a.m_engine = nullptr;
			}

			return *this;
		}

		Audio::~Audio() noexcept
		{
			if (m_source != nullptr && m_engine != nullptr)
			{
				m_engine->removeSoundSource(m_source);
			}

			if (m_engine != nullptr)
			{
				m_engine->drop();
				m_engine = nullptr;
			}
		}

		void Audio::play(const bool loop) noexcept
		{
			m_engine->play2D(m_source, loop);
		}

		void Audio::stop() noexcept
		{
			m_engine->stopAllSoundsOfSoundSource(m_source);
		}

		bool Audio::is_playing() const noexcept
		{
			return m_engine->isCurrentlyPlaying(m_source);
		}
	} // namespace audio
} // namespace galaxy