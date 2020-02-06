///
/// SpriteComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SpriteComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteComponent::SpriteComponent() noexcept
		:m_name(""), m_layer(0), m_colour(0, 0, 0, 255)
	{
	}

	SpriteComponent::SpriteComponent(const std::string& name, const unsigned int layer, const sf::Color col, const protostar::Rect<float> texRect) noexcept
		:m_name(name), m_layer(layer), m_colour(col), m_texRect(texRect)
	{
	}
}