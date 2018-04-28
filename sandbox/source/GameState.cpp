///
/// GameState.cpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/systems/RenderSystem.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "GameState.hpp"

using namespace sl;

GameState::~GameState()
{
}

void GameState::load()
{
	/*
	entt::Entity entity = Locator::world->m_registry.create();
	Locator::world->m_registry.assign<SpriteComponent>(entity, "bg", 1.0f);
	Locator::world->m_registry.assign<TransformComponent>(entity, 0, 0, Rect<float, int>{0, 0, 896, 576});
	Locator::world->m_registry.assign<RenderComponent>(entity);
	auto& rc = Locator::world->m_registry.get<RenderComponent>(entity);
	rc.m_renderTypes.push_back(RenderTypes::SPRITE);
	*/

	m_gameLevel = std::make_unique<GameLevel>(sl::Rect<float, int>{ 0, 0, 896, 576 });
	Locator::world->m_currentLevel = m_gameLevel.get();

	/*
	Locator::world->m_registry.sort<TransformComponent>([](const auto& lhs, const auto& rhs)
	{
		return lhs.m_layer < rhs.m_layer;
	});
	*/
}

void GameState::unload()
{
	Locator::world->m_registry.reset();

	m_gameLevel.reset();
	Locator::world->m_currentLevel = nullptr;
}

void GameState::event(ALLEGRO_EVENT* event)
{
	// escape to close window
}

void GameState::update(double dt)
{
}

void GameState::render()
{
	Locator::world->getSystem<RenderSystem>()->render(Locator::world->m_registry);
}