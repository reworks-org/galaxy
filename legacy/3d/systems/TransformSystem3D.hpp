///
/// TransformSystem3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_TRANSFORMSYSTEM3D_HPP_
#define GALAXY_SYSTEM_TRANSFORMSYSTEM3D_HPP_

#include "galaxy/ecs/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Handles 3D transformations being applied to entities.
		///
		class TransformSystem3D final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			TransformSystem3D() noexcept;

			///
			/// Destructor.
			///
			virtual ~TransformSystem3D() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt "Lag" from gameloop.
			///
			void update(core::World& world, const double dt) override;
		};
	} // namespace systems
} // namespace galaxy

#endif