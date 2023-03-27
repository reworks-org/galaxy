///
/// ScriptSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_SCRIPTSYSTEM_HPP_
#define GALAXY_SYSTEMS_SCRIPTSYSTEM_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Update entities with a script component.
		///
		class ScriptSystem final : public systems::System
		{
		public:
			///
			/// Constructor.
			///
			ScriptSystem();

			///
			/// Destructor.
			///
			virtual ~ScriptSystem();

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