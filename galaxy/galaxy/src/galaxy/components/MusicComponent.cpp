///
/// MusicComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "MusicComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	MusicComponent::MusicComponent() noexcept
	    : m_music {}
	{
	}

	MusicComponent::MusicComponent(const nlohmann::json& json)
	    : m_music {}
	{
		if (m_music.load(json.at("file")))
		{
			m_music.set_looping(json.at("loop"));
			m_music.set_pitch(json.at("pitch"));
		}
		else
		{
			PL_LOG(PL_ERROR, "Unable to load file: {0}.", std::string {json.at("file")});
		}
	}

	MusicComponent::MusicComponent(MusicComponent&& mc)
	{
		this->m_music = std::move(mc.m_music);
	}

	MusicComponent& MusicComponent::operator=(MusicComponent&& mc)
	{
		if (this != &mc)
		{
			this->m_music = std::move(mc.m_music);
		}

		return *this;
	}

	MusicComponent::~MusicComponent()
	{
		m_music.stop();
	}
} // namespace galaxy