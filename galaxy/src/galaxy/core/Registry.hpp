///
/// Registry.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_REGISTRY_HPP_
#define GALAXY_CORE_REGISTRY_HPP_

#include <box2d/b2_world.h>
#include <entt/entity/registry.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Ellipse.hpp"
#include "galaxy/components/GUI.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Polygon.hpp"
#include "galaxy/components/Polyline.hpp"
#include "galaxy/components/RenderCommand.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/shapes/Shape.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Wrapper around entt::registry to expand functionality.
		///
		class Registry final
		{
			///
			/// Typedef for creating b2 bodies from components.
			///
			using B2BodyConstruction = meta::vector<std::pair<components::RigidBody*, components::Transform*>>;

		  public:
			///
			/// Constructor.
			///
			Registry();

			///
			/// Destructor.
			///
			~Registry();

			///
			/// \brief Create an entity with some default components.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create();

			///
			/// Create an entity from a prefab.
			///
			/// \param name Name of the prefab to load.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create_from_prefab(const std::string& name);

			///
			/// Validate an entity to make sure all components have met their requirements as defined by register_dependencies().
			///
			/// \param entity Entity to validate.
			///
			/// \return True if entity is valid.
			///
			[[nodiscard]] bool is_valid(const entt::entity entity);

			///
			/// \brief Updates pending component data.
			///
			/// For example, box2d bodies.
			///
			/// \param b2World Box2d world data.
			///
			void update(b2World& b2World);

			///
			/// Clear any pending data.
			///
			void clear();

		  private:
			///
			/// Function that integrates a box2d construction with entt.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_rigidbody(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates a box2d destruction with entt.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destroy_rigidbody(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates lua init with entt on construct event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_script(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates lua destroy with entt on destruction event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destruct_script(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates nuklear init with entt on construct event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_nui(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates nuklear destroy with entt on destruction event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destruct_nui(entt::registry& registry, entt::entity entity);

			///
			/// Function that runs when an entity is enabled.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void enable_entity(entt::registry& registry, entt::entity entity);

			///
			/// Function that runs when an entity is disabled.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void disable_entity(entt::registry& registry, entt::entity entity);

			///
			/// Adds a renderable object to the registry.
			///
			/// \tparam GraphicsComponent Component that can be rendered.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			template<typename GraphicsComponent>
			void construct_rendercommand(entt::registry& registry, entt::entity entity);

		  public:
			///
			/// Scene entities.
			///
			entt::registry m_entt;

		  private:
			///
			/// List of rigid bodies that need to be constructed.
			///
			B2BodyConstruction m_bodies_to_construct;
		};

		template<typename GraphicsComponent>
		inline void Registry::construct_rendercommand(entt::registry& registry, entt::entity entity)
		{
			auto& cmd = registry.emplace_or_replace<components::RenderCommand>(entity).m_command;

			if constexpr ((std::is_same<GraphicsComponent, components::Circle>::value) || (std::is_same<GraphicsComponent, components::Ellipse>::value) ||
						  (std::is_same<GraphicsComponent, components::Point>::value) || (std::is_same<GraphicsComponent, components::Polygon>::value) ||
						  (std::is_same<GraphicsComponent, components::Polyline>::value))
			{
				graphics::Shape* shape = &registry.get<GraphicsComponent>(entity).m_shape;

				cmd.count             = shape->vao().count();
				cmd.instances         = shape->vao().instances();
				cmd.mode              = shape->mode();
				cmd.offset            = shape->vao().offset();
				cmd.vao               = shape->vao().id();
				cmd.uniforms.colour   = shape->m_colour.vec4();
				cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
				cmd.uniforms.handle   = 0;
				cmd.uniforms.textured = false;
				cmd.uniforms.point    = shape->mode() == GL_POINTS ? true : false;
			}
			else if constexpr (std::is_same<GraphicsComponent, components::Sprite>::value)
			{
				auto& sprite = registry.get<components::Sprite>(entity);

				cmd.count             = sprite.m_vao.count();
				cmd.instances         = sprite.m_vao.instances();
				cmd.mode              = GL_TRIANGLES;
				cmd.offset            = sprite.m_vao.offset();
				cmd.vao               = sprite.m_vao.id();
				cmd.uniforms.colour   = sprite.m_tint.vec4();
				cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
				cmd.uniforms.handle   = sprite.get_texture()->handle();
				cmd.uniforms.textured = true;
				cmd.uniforms.point    = false;
			}
			else if constexpr (std::is_same<GraphicsComponent, components::Text>::value)
			{
				auto& text = registry.get<components::Text>(entity);

				cmd.count             = text.m_text.vao().count();
				cmd.instances         = text.m_text.vao().instances();
				cmd.mode              = GL_TRIANGLES;
				cmd.offset            = text.m_text.vao().offset();
				cmd.vao               = text.m_text.vao().id();
				cmd.uniforms.colour   = text.m_text.m_colour.vec4();
				cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
				cmd.uniforms.handle   = text.m_text.render_texture().handle();
				cmd.uniforms.textured = true;
				cmd.uniforms.point    = false;
			}
		}
	} // namespace core
} // namespace galaxy

#endif
