///
/// ScrollingBackgroundComponent.hpp
/// galaxy
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCROLLINGBACKGROUNDCOMPONENT_HPP_
#define GALAXY_SCROLLINGBACKGROUNDCOMPONENT_HPP_

#include "galaxy/libs/sol2/forward.hpp"
#include "galaxy/libs/entt/entity/registry.hpp"

namespace galaxy
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
		explicit ScrollingBackgroundComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param speed Speed to move the background at.
		///
		explicit ScrollingBackgroundComponent(const float speed);

		///
		/// Destructor.
		///
		~ScrollingBackgroundComponent() noexcept;

		///
		/// Call this to disable the internal second entity.
		///
		void disableInternalEntity();

	private:
		///
		/// Speed to move the background at. Any negatvie value is converted to a postitive one.
		///
		float m_speed;
		
		///
		/// The secondary entity that keeps track of the second lot of texture data to render.
		///
		entt::DefaultRegistry::entity_type m_secondEntity;
	};
}

#endif