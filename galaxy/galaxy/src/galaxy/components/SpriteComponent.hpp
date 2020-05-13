///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITECOMPONENT_HPP_
#define GALAXY_SPRITECOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/renderer/Renderable.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Allows for an entity to be rendered.
	///
	class SpriteComponent final : public qs::Renderable
	{
	public:
		///
		/// Default constructor.
		///
		SpriteComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SpriteComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~SpriteComponent() noexcept;
	};
}

#endif