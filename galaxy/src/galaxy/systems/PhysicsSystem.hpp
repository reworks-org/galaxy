///
/// PhysicsSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_PHYSICSSYSTEM_HPP_
#define GALAXY_SYSTEMS_PHYSICSSYSTEM_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Update entities with a script component.
		///
		class PhysicsSystem final : public systems::System
		{
		public:
			///
			/// Constructor.
			///
			PhysicsSystem() noexcept;

			///
			/// Destructor.
			///
			virtual ~PhysicsSystem() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param layer Non-owning pointer to current layer.
			///
			void update(state::Layer* layer) override;
		};
	} // namespace systems
} // namespace galaxy

#endif