///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SpriteComponent_HPP_
#define GALAXY_SpriteComponent_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/sprite/Sprite.hpp>

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
		/// Constructor.
		///
		SpriteComponent();

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SpriteComponent(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~SpriteComponent() noexcept = default;

		///
		/// Sprite object.
		///
		qs::Sprite m_sprite;
	};
} // namespace galaxy

#endif