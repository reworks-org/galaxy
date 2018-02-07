///
/// SpriteComponent.hpp
/// starlight
///
/// Created by reworks on 12/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SPRITECOMPONENT_HPP_
#define STARLIGHT_SPRITECOMPONENT_HPP_

#include "sol2/sol_forward.hpp"

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
		/// Alternate constructor.
		///
		/// \param spriteName Name of the sprite in the atlas.
		///
		SpriteComponent(const std::string& spriteName);

		///
		/// Destructor.
		///
		~SpriteComponent() = default;

		///
		/// Default move assignment overload.
		///
		SpriteComponent& operator=(const SpriteComponent&);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		SpriteComponent() = delete;

	public:
		std::string m_spriteName;
	};
}

#endif