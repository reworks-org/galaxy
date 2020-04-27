///
/// MusicComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/fs/PhysfsInputStream.hpp"

#include "MusicComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	MusicComponent::MusicComponent(const std::string& file) noexcept
	{
		load(file);
	}

	void MusicComponent::load(const std::string& file) noexcept
	{
		m_music.openFromStream(galaxy::PhysfsInputStream(file));
	}
}