///
/// AudioComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "AudioComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	AudioComponent::AudioComponent() noexcept
	{
	}

	AudioComponent::AudioComponent(const nlohmann::json& json) noexcept
	{
		m_audio.load(json.at("file"));
		m_audio.setLooping(json.at("looping"));
	}

	AudioComponent::~AudioComponent() noexcept
	{
	}
}