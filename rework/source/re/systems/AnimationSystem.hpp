///
/// AnimationSystem.hpp
/// rework
///
///  Created by reworks on 10/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_ANIMATIONSYSTEM_HPP_
#define REWORK_ANIMATIONSYSTEM_HPP_

#include "re/types/System.hpp"

namespace re
{
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
		~AnimationSystem() override = default;

	private:
		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registery) override;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		AnimationSystem(const AnimationSystem&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		AnimationSystem(AnimationSystem&&) = delete;
	};
}

#endif