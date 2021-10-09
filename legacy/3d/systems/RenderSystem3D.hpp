///
/// RenderSystem3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_RENDERSYSTEM3D_HPP_
#define GALAXY_SYSTEMS_RENDERSYSTEM3D_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/ecs/System.hpp"
#include "galaxy/graphics/camera/Camera3D.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// System that handles rendering of entities with a 3d model.
		///
		class RenderSystem3D final : public ecs::System
		{
			friend class core::World;

		public:
			///
			/// Constructor.
			///
			RenderSystem3D() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~RenderSystem3D() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt DeltaTime from gameloop.
			///
			void update(core::World& world, const double dt) override;

			///
			/// Render shadows.
			///
			/// \param world Game World containing entities.
			/// \param camera Camera used for rendering.
			///
			void render_shadows(core::World& world, graphics::Camera3D& camera);

			///
			/// Render models.
			///
			/// \param world Game World containing entities.
			/// \param camera Camera used for rendering.
			///
			void render(core::World& world, graphics::Camera3D& camera);
		};
	} // namespace systems
} // namespace galaxy

#endif