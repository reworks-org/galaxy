///
/// SoundComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "SoundComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SoundComponent::SoundComponent()
	    : m_sound {}
	{
	}

	SoundComponent::SoundComponent(const nlohmann::json& json)
	    : m_sound {}
	{
		if (m_sound.load(json.at("file")))
		{
			m_sound.set_looping(json.at("looping"));
			m_sound.set_pitch(json.at("pitch"));
		}
		else
		{
			PL_LOG(PL_ERROR, "Failed to load sound effect: {0}.", std::string {json.at("file")});
		}
	}

	SoundComponent::SoundComponent(SoundComponent&& sc)
	{
		this->m_sound = std::move(sc.m_sound);
	}

	SoundComponent& SoundComponent::operator=(SoundComponent&& sc)
	{
		if (this != &sc)
		{
			this->m_sound = std::move(sc.m_sound);
		}

		return *this;
	}

	SoundComponent::~SoundComponent()
	{
		m_sound.stop();
	}
} // namespace galaxy