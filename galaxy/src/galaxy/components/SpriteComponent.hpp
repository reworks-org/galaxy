///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITECOMPONENT_HPP_
#define GALAXY_SPRITECOMPONENT_HPP_

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
		SpriteComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		/// 
		explicit SpriteComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param opacity Opacity of Texture / Sprite Texture. From 0.0f - 1.0f.
		/// \param atlasID Name of the sprite in the texture atlas.
		///
		SpriteComponent(const float opacity, const std::string& atlasID);

		///
		/// Destructor.
		///
		~SpriteComponent() noexcept = default;

	private:
		unsigned int m_layer;
	};
}

#endif