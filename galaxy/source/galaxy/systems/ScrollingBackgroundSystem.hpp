///
/// ScrollingBackgroundSystem.hpp
/// galaxy
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCROLLINGBACKGROUNDSYSTEM_HPP_
#define GALAXY_SCROLLINGBACKGROUNDSYSTEM_HPP_

#include "galaxy/types/System.hpp"

galaxy
{
	///
	/// Process entities with an ScrollingBackgroundComponent.
	///
	class ScrollingBackgroundSystem final : public System
	{
	public:
		///
		/// Constructor.
		///
		ScrollingBackgroundSystem() = default;

		///
		/// Destructor
		///
		~ScrollingBackgroundSystem() noexcept override = default;

	private:
		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		/// \param dt Delta Time from update loop.
		/// \param registry Default entity registry.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;
	};
}

#endif