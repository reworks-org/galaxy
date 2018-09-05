///
/// Sol2Interface.hpp
/// starlight
///
/// Created by reworks on 20/08/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SOL2INTERFACE_HPP_
#define STARLIGHT_SOL2INTERFACE_HPP_

#include "sl/core/World.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/entity/registry.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/components/ScrollingBackgroundComponent.hpp"

namespace sl
{
	///
	/// Helper functions to bridge various code modules and sol2.
	///
	namespace Sol2Interface
	{
		///
		/// Workaround function to allow sol2 to destroy entt entities.
		///
		/// \param entity Entity to destroy.
		///
		inline void enttDestroyWorkaround(entt::DefaultRegistry::entity_type entity)
		{
			sl::Locator::world->m_registry.destroy(entity);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignEnabledComponent(entt::DefaultRegistry::entity_type entity)
		{
			sl::Locator::world->m_registry.assign<EnabledComponent>(entity);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignTransformComponent(entt::DefaultRegistry::entity_type entity, int layer, float angle, const Rect<float, int>& rect)
		{
			sl::Locator::world->m_registry.assign<TransformComponent>(entity, layer, angle, rect);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignSBComponent(entt::DefaultRegistry::entity_type entity, float speed)
		{
			sl::Locator::world->m_registry.assign<ScrollingBackgroundComponent>(entity, speed);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignRenderComponent(entt::DefaultRegistry::entity_type entity, float opacity, const std::string& atlasID)
		{
			sl::Locator::world->m_registry.assign<RenderComponent>(entity, opacity, atlasID);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignParticleComponent(entt::DefaultRegistry::entity_type entity, float dx, float dy, float fade)
		{
			sl::Locator::world->m_registry.assign<ParticleComponent>(entity, dx, dy, fade);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignParallaxComponent(entt::DefaultRegistry::entity_type entity, float verticalSpeed, float horizontalSpeed)
		{
			sl::Locator::world->m_registry.assign<ParallaxComponent>(entity, verticalSpeed, horizontalSpeed);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignAnimationComponent(entt::DefaultRegistry::entity_type entity, tmx_map* map, tmx_tile* tile, int x, int y, int tileWidth, int tileHeight, const std::string& layerName)
		{
			sl::Locator::world->m_registry.assign<AnimationComponent>(entity, map, tile, x, y, tileWidth, tileHeight, layerName);
		}
	}
}

#endif