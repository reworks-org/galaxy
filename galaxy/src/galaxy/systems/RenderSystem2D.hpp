///
/// RenderSystem2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_RENDERSYSTEM2D_HPP_
#define GALAXY_SYSTEMS_RENDERSYSTEM2D_HPP_

#include <mutex>

#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Camera2D.hpp"

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
			RenderSystem2D() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~RenderSystem2D() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt DeltaTime from gameloop.
			///
			void update(core::World& world, const double dt) override;

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
			/// BatchSprite cache.
			///
			std::vector<std::pair<components::BatchSprite*, components::Transform2D*>> m_batchsprites;

			///
			/// Point lock.
			///
			std::mutex m_point_lock;

			///
			/// Line lock.
			///
			std::mutex m_line_lock;

			///
			/// LineLoop lock.
			///
			std::mutex m_lineloop_lock;

			///
			/// Text lock.
			///
			std::mutex m_text_lock;

			///
			/// Sprite lock.
			///
			std::mutex m_sprite_lock;

			///
			/// Batch Sprite lock.
			///
			std::mutex m_batch_lock;
		};
	} // namespace systems
} // namespace galaxy

#endif