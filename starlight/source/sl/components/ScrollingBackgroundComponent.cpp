///
/// ScrollingBackgroundComponent.cpp
/// starlight
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ScrollingBackgroundComponent.hpp"

namespace sl
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
		if (sl::Locator::world->m_registry.valid(m_secondEntity))
		{
			sl::Locator::world->m_registry.destroy(m_secondEntity);
		}
	}

	void ScrollingBackgroundComponent::disableInternalEntity()
	{
		sl::Locator::world->m_registry.remove<EnabledComponent>(m_secondEntity);
	}
}