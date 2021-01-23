///
/// RenderSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_RENDERSYSTEM_HPP_
#define GALAXY_SYSTEMS_RENDERSYSTEM_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/ecs/System.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Contains information needed to render an object with OpenGL.
		///
		struct RenderData final
		{
			///
			/// The Entity.
			///
			ecs::Entity m_entity;

			///
			/// The type of renderable.
			///
			graphics::Renderables m_type;

			///
			/// The z level.
			///
			int m_z_level = 0;

			///
			/// Pointer to a transformation.
			///
			components::Transform* m_transform = nullptr;
		};

		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class RenderSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			RenderSystem() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~RenderSystem() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param dt DeltaTime from gameloop.
			///
			void update(const double dt) override;

			///
			/// Render sprites / textures to screen.
			///
			/// \param world Game World containing entities to render.
			/// \param camera Camera used for rendering.
			///
			void render(core::World& world, graphics::Camera& camera);

		private:
			///
			/// Stored vector of renderdata to pass to renderer.
			///
			std::vector<RenderData> m_sorted;
		};
	} // namespace systems
} // namespace galaxy

#endif