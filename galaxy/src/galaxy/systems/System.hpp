///
/// System.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_SYSTEM_HPP_
#define GALAXY_SYSTEMS_SYSTEM_HPP_

#include "galaxy/state/Scene.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Represents a system that operates on sets of components.
		///
		class System
		{
		public:
			///
			/// Destructor.
			///
			virtual ~System() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Currently active scene.
			///
			virtual void update(std::shared_ptr<state::Scene> scene) = 0;

		protected:
			///
			/// Constructor.
			///
			System() noexcept = default;
		};
	} // namespace systems
} // namespace galaxy

#endif