///
/// Sol2Interface.hpp
/// galaxy
///
/// Created by reworks on 20/08/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SOL2INTERFACE_HPP_
#define GALAXY_SOL2INTERFACE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/libs/entt/entity/registry.hpp"
#include "galaxy/components/RenderComponent.hpp"
#include "galaxy/components/EnabledComponent.hpp"
#include "galaxy/components/ParticleComponent.hpp"
#include "galaxy/components/ParallaxComponent.hpp"
#include "galaxy/components/AnimationComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"
#include "galaxy/components/ScrollingBackgroundComponent.hpp"

namespace galaxy
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
		inline void enttDestroyWorkaround(const entt::DefaultRegistry::entity_type entity)
		{
			galaxy::Locator::world->m_registry.destroy(entity);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignEnabledComponent(const entt::DefaultRegistry::entity_type entity)
		{
			galaxy::Locator::world->m_registry.assign<EnabledComponent>(entity);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignTransformComponent(const entt::DefaultRegistry::entity_type entity, const int layer, const float angle, const Rect<float, int>& rect)
		{
			galaxy::Locator::world->m_registry.assign<TransformComponent>(entity, layer, angle, rect);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignSBComponent(const entt::DefaultRegistry::entity_type entity, const float speed)
		{
			galaxy::Locator::world->m_registry.assign<ScrollingBackgroundComponent>(entity, speed);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignRenderComponent(const entt::DefaultRegistry::entity_type entity, const float opacity, const std::string& atlasID)
		{
			galaxy::Locator::world->m_registry.assign<RenderComponent>(entity, opacity, atlasID);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignParticleComponent(const entt::DefaultRegistry::entity_type entity, const float dx, const float dy, const float fade)
		{
			galaxy::Locator::world->m_registry.assign<ParticleComponent>(entity, dx, dy, fade);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignParallaxComponent(const entt::DefaultRegistry::entity_type entity, const float verticalSpeed, const float horizontalSpeed)
		{
			galaxy::Locator::world->m_registry.assign<ParallaxComponent>(entity, verticalSpeed, horizontalSpeed);
		}

		///
		/// Workaround function to allow sol2 to assign components to entt entities.
		///
		inline void assignAnimationComponent(const entt::DefaultRegistry::entity_type entity, const sol::table& table)
		{
			galaxy::Locator::world->m_registry.assign<AnimationComponent>(entity, table);
		}
	}
}

#endif