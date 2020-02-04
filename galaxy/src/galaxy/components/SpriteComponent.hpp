///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITECOMPONENT_HPP_
#define GALAXY_SPRITECOMPONENT_HPP_

#include <string>

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
		/// \param opacity Opacity of the sprite. From 0.0f - 1.0f.
		///
		explicit SpriteComponent(const std::string& name, const unsigned int layer, const float opacity = 1.0f) noexcept;

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
		float m_opacity;
	};
}

#endif