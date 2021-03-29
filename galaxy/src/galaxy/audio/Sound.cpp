///
/// Sound.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace audio
	{
		Sound::Sound() noexcept
		    : Buffer {}, SourceManipulator {}, m_source {}
		{
		}

		Sound::Sound(std::string_view file)
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load sound file: {0}.", file);
			}
		}

		Sound::Sound(const nlohmann::json& json)
		{
			if (load(json.at("file")))
			{
				set_looping(json.at("looping"));
				set_gain(json.at("volume"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load sound effect: {0}.", std::string {json.at("file")});
			}
		}

		void Sound::play()
		{
			alSourcePlay(m_source.handle());
		}

		void Sound::pause()
		{
			alSourcePause(m_source.handle());
		}

		void Sound::stop()
		{
			alSourceStop(m_source.handle());
			alSourceRewind(m_source.handle());
		}

		const bool Sound::load(std::string_view file)
		{
			const auto res = internal_load(file);
			if (res)
			{
				set_max_distance(1000.0f);
				m_source.queue(this);
			}

			return res;
		}

		void Sound::set_looping(const bool looping)
		{
			alSourcei(m_source.handle(), AL_LOOPING, looping);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source looping."));
			}
		}

		const bool Sound::get_looping()
		{
			int looping = 0;

			alGetSourcei(m_source.handle(), AL_LOOPING, &looping);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source looping."));
			}

			return static_cast<bool>(looping);
		}
	} // namespace audio
} // namespace galaxy