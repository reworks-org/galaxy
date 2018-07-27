///
/// ScrollingBackgroundComponent.hpp
/// starlight
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SCROLLINGBACKGROUNDCOMPONENT_HPP_
#define STARLIGHT_SCROLLINGBACKGROUNDCOMPONENT_HPP_

#include "sl/libs/sol2/sol_forward.hpp"
#include "sl/libs/entt/entity/registry.hpp"

namespace sl
{
	///
	/// Stores data for the entitys scrolling background.
	///
	class ScrollingBackgroundComponent final
	{
		friend class ScrollingBackgroundSystem;
	public:
		///
		/// \brief Default Constructor.
		///
		/// m_speed is default-initialized to 1.0f, and the secondEntity constructed to an empty state.
		///
		ScrollingBackgroundComponent();
		
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		ScrollingBackgroundComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param speed Speed to move the background at.
		///
		ScrollingBackgroundComponent(float speed);

		///
		/// Destructor.
		///
		~ScrollingBackgroundComponent() = default;

		///
		/// Default move assignment overload.
		///
		ScrollingBackgroundComponent& operator=(const ScrollingBackgroundComponent&);

	private:
		///
		/// The secondary entity that keeps track of the second lot of texture data to render.
		///
		entt::DefaultRegistry::entity_type m_secondEntity;

		///
		/// Speed to move the background at.
		///
		float m_speed;
	};
}

#endif