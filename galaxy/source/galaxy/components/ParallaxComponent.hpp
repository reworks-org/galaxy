///
/// ParallaxComponent.hpp
/// galaxy
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PARALLAXCOMPONENT_HPP_
#define GALAXY_PARALLAXCOMPONENT_HPP_

#include "galaxy/libs/sol2/forward.hpp"

namespace galaxy
{
	///
	/// Stores data for the entitys parallax texture.
	///
	class ParallaxComponent final
	{
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		ParallaxComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		explicit ParallaxComponent(const sol::table& table);

		///
		/// Constructor.
		///
		/// \param verticalSpeed Speed to move background vertically.
		/// \param horizontalSpeed Speed to move background horizontally.
		///
		ParallaxComponent(const float verticalSpeed, const float horizontalSpeed);

		///
		/// Destructor.
		///
		~ParallaxComponent() noexcept = default;

	public:
		///
		/// Vertical speed to move the background at.
		///
		float m_verticalSpeed;

		///
		/// Horizontal speed to move the background at.
		///
		float m_horizontalSpeed;
	};
}

#endif