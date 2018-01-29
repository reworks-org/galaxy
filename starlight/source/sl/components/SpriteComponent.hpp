///
///  SpriteComponent.hpp
///  starlight
///
///  Created by reworks on 12/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SPRITECOMPONENT_HPP_
#define STARLIGHT_SPRITECOMPONENT_HPP_

#include "sol2/sol_forward.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	class SpriteComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SpriteComponent(const sol::table& table);

		///
		/// Destructor.
		///
		~SpriteComponent() = default;

	public:
		entt::HashedString m_spriteName;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		SpriteComponent() = delete;
	};
}

#endif