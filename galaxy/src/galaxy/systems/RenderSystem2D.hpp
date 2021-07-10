///
/// RenderSystem2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_RENDERSYSTEM2D_HPP_
#define GALAXY_SYSTEMS_RENDERSYSTEM2D_HPP_

#include "galaxy/core/Scene2D.hpp"
#include "galaxy/math/QuadTree.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class RenderSystem2D final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			RenderSystem2D() noexcept;

			///
			/// Destructor.
			///
			virtual ~RenderSystem2D() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Currently active scene.
			/// \param dt DeltaTime from gameloop.
			///
			void update(core::Scene2D* scene, const double dt) override;

			///
			/// Render sprites / textures to screen.
			///
			/// \param world Game World containing entities.
			/// \param camera Camera used for rendering.
			///
			void render(core::World& world, graphics::Camera2D& camera);

		private:
			///
			/// Quadtree for spacial partitioning.
			///
			math::Quadtree m_quadtree;

			///
			/// Output memory cache.
			///
			std::vector<math::Quadtree::Object*> m_output;
		};
	} // namespace systems
} // namespace galaxy

#endif