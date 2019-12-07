///
/// ScrollingBackgroundComponent.cpp
/// galaxy
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/components/RenderComponent.hpp"
#include "galaxy/components/EnabledComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "ScrollingBackgroundComponent.hpp"

namespace galaxy
{
	ScrollingBackgroundComponent::ScrollingBackgroundComponent()
		:m_speed(1.0f)
	{
		// Entity data for the second background is created from the primary component in the scrolling background system.
		m_secondEntity = Locator::world->m_registry.create();
		Locator::world->m_registry.assign<RenderComponent>(m_secondEntity);
		Locator::world->m_registry.assign<TransformComponent>(m_secondEntity);
		Locator::world->m_registry.assign<EnabledComponent>(m_secondEntity);
	}

	ScrollingBackgroundComponent::ScrollingBackgroundComponent(const sol::table& table)
		:m_speed(1.0f)
	{
		// Correct to positive value.
		m_speed = std::abs(table.get<float>("speed"));

		// Entity data for the second background is created from the primary component in the scrolling background system.
		m_secondEntity = Locator::world->m_registry.create();
		Locator::world->m_registry.assign<RenderComponent>(m_secondEntity);
		Locator::world->m_registry.assign<TransformComponent>(m_secondEntity);
		Locator::world->m_registry.assign<EnabledComponent>(m_secondEntity);
	}

	ScrollingBackgroundComponent::ScrollingBackgroundComponent(const float speed)
		:m_speed(speed)
	{
		// Correct to positive value.
		m_speed = std::abs(m_speed);

		// Entity data for the second background is created from the primary component in the scrolling background system.
		m_secondEntity = Locator::world->m_registry.create();
		Locator::world->m_registry.assign<RenderComponent>(m_secondEntity);
		Locator::world->m_registry.assign<TransformComponent>(m_secondEntity);
		Locator::world->m_registry.assign<EnabledComponent>(m_secondEntity);
	}

	ScrollingBackgroundComponent::~ScrollingBackgroundComponent()
	{
		if (galaxyLocator::world->m_registry.valid(m_secondEntity))
		{
			galaxyLocator::world->m_registry.destroy(m_secondEntity);
		}
	}

	void ScrollingBackgroundComponent::disableInternalEntity()
	{
		galaxyLocator::world->m_registry.remove<EnabledComponent>(m_secondEntity);
	}
}