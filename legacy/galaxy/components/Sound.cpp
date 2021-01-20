///
/// Sound.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace audio
	{
		Sound::Sound()
		    : Buffer {}, SourceManipulator {}, m_source {}
		{
			set_source_to_manipulate(m_source.handle());
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
				set_pitch(json.at("pitch"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load sound effect: {0}.", std::string {json.at("file")});
			}
		}

		Sound::Sound(Sound&& s)
		{
			this->m_source = std::move(s.m_source);
		}

		Sound& Sound::operator=(Sound&& s)
		{
			if (this != &s)
			{
				this->m_source = std::move(s.m_source);
			}

			return *this;
		}

		bool Sound::load(std::string_view file)
		{
			const auto res = internal_load(file);
			if (res)
			{
				m_source.queue(this);
			}

			return res;
		}
	} // namespace audio
} // namespace galaxy