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

#include "sl/libs/sol2/sol_forward.hpp"

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