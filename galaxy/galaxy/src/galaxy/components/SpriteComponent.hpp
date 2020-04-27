///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITECOMPONENT_HPP_
#define GALAXY_SPRITECOMPONENT_HPP_

#include <array>
#include <string>

#include <SFML/Graphics/Vertex.hpp>
#include <protostar/shapes/Rect.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Allows for an entity to be rendered.
	///
	class SpriteComponent final
	{
	public:
		///
		/// Default constructor.
		///
		SpriteComponent() noexcept;

		///
		/// Argument constructor.
		///
		/// \param name Name of the sprite in the texture atlas.
		/// \param layer Z-Level of sprite.
		/// \param col Colour of sprite. Mostly for opacity.
		/// \param texRect Texture rectangle on atlas.
		///
		explicit SpriteComponent(const std::string& name, const unsigned int layer, const sf::Color col, const protostar::Rect<float> texRect) noexcept;

		///
		/// Destructor.
		///
		~SpriteComponent() noexcept = default;

	public:
		///
		/// Texture Atlas ID.
		///
		std::string m_name;

		///
		/// Z-level.
		///
		unsigned int m_layer;

		///
		/// Opacity. 0.0f - 1.0f.
		///
		sf::Color m_colour;

		///
		/// Texture rectangle.
		///
		protostar::Rect<float> m_texRect;
	};
}

#endif