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
	namespace components
	{
		class BatchSprite;
		class Primitive2D;
		class Sprite;
		class Text;
		class Transform2D;
	} // namespace components

	namespace systems
	{
		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class RenderSystem2D final : public ecs::System
		{
			friend class core::World;

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
			/// Point cache.
			///
			std::vector<std::pair<components::Primitive2D*, components::Transform2D*>> m_points;

			///
			/// Line cache.
			///
			std::vector<std::pair<components::Primitive2D*, components::Transform2D*>> m_lines;

			///
			/// LineLoop cache.
			///
			std::vector<std::pair<components::Primitive2D*, components::Transform2D*>> m_lineloops;

			///
			/// Text cache.
			///
			std::vector<std::pair<components::Text*, components::Transform2D*>> m_text;

			///
			/// Sprite cache.
			///
			std::vector<std::pair<components::Sprite*, components::Transform2D*>> m_sprites;

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