///
/// ScrollingBackgroundSystem.hpp
/// starlight
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SCROLLINGBACKGROUNDSYSTEM_HPP_
#define STARLIGHT_SCROLLINGBACKGROUNDSYSTEM_HPP_

#include "sl/types/System.hpp"

namespace sl
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