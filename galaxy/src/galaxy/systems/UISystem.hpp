///
/// UISystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_UISYSTEM_HPP_
#define GALAXY_SYSTEMS_UISYSTEM_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Update entities with an rml ui component.
		///
		class UISystem final : public systems::System
		{
		public:
			///
			/// Constructor.
			///
			UISystem();

			///
			/// Destructor.
			///
			virtual ~UISystem();

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Non-owning pointer to current scene.
			///
			void update(scene::Scene* scene) override;
		};
	} // namespace systems
} // namespace galaxy

#endif