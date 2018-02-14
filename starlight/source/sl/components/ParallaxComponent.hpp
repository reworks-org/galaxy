///
/// ParallaxComponent.hpp
/// starlight
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARALLAXCOMPONENT_HPP_
#define STARLIGHT_PARALLAXCOMPONENT_HPP_

#include <string>

#include "sol2/sol_forward.hpp"

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class ParallaxComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param entity Entity to attach to component fixtures.
		/// \param table sol::table containing data.
		///
		ParallaxComponent(entt::Entity entity, const sol::table& table);

		///
		/// Destructor.
		///
		~ParallaxComponent() = default;

		///
		/// Default move assignment overload.
		///
		ParallaxComponent& operator=(const ParallaxComponent&);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		ParallaxComponent() = delete;

	public:
		float m_verticalSpeed;
		float m_horizontalSpeed;
	};
}

#endif