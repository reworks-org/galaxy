///
/// ParallaxComponent.hpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARALLAXCOMPONENT_HPP_
#define STARLIGHT_PARALLAXCOMPONENT_HPP_

#include "sl/math/Vector2.hpp"
#include "sol2/sol_forward.hpp"
#include "entt/core/hashed_string.hpp"
#include "sl/graphics/ParallaxSprite.hpp"

namespace sl
{
	class ParallaxComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		/// 
		ParallaxComponent(const sol::table& table);

		///
		/// Move Constructor.
		///
		ParallaxComponent(ParallaxComponent&&) = default;

		///
		/// Destructor.
		///
		~ParallaxComponent() = default;

	public:
		std::vector<ParallaxSprite> m_parallaxSprites;

		///
		/// Default constructor.
		/// Deleted.
		///
		ParallaxComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ParallaxComponent(const ParallaxComponent&) = delete;
	};
}


#endif