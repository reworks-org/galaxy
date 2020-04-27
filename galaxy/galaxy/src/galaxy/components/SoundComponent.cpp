///
/// SoundComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/fs/PhysfsInputStream.hpp"

#include "SoundComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SoundComponent::SoundComponent(const std::string& file) noexcept
	{
		load(file);
	}

	void SoundComponent::load(const std::string& file) noexcept
	{
		m_buffer.loadFromStream(galaxy::PhysfsInputStream(file));
		m_sound.setBuffer(m_buffer);
	}
}