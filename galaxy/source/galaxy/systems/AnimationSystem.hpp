///
/// AnimationSystem.hpp
/// galaxy
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ANIMATIONSYSTEM_HPP_
#define GALAXY_ANIMATIONSYSTEM_HPP_

#include "galaxy/types/System.hpp"

galaxy
{
	///
	/// Process entities with an AnimationComponent.
	///
	class AnimationSystem final : public System
	{
	public:
		///
		/// Constructor.
		///
		AnimationSystem() = default;

		///
		/// Destructor
		///
		~AnimationSystem() noexcept override = default;

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