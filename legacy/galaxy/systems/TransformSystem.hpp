///
/// TransformSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_TRANSFORMSYSTEM_HPP_
#define GALAXY_SYSTEM_TRANSFORMSYSTEM_HPP_

#include "galaxy/core/Scene2D.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Handles transformations being applied to entities.
		///
		class TransformSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			TransformSystem() noexcept;

			///
			/// Destructor.
			///
			virtual ~TransformSystem() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Currently active scene.
			///
			void update(core::Scene2D* scene) override;
		};
	} // namespace systems
} // namespace galaxy

#endif