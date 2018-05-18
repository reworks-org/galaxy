///
/// GameState.cpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <sl/core/World.hpp>
#include <sl/tags/CameraTag.hpp>
#include <sl/core/ServiceLocator.hpp>
#include <sl/systems/RenderSystem.hpp>
#include <sl/components/RenderComponent.hpp>
#include <sl/components/TransformComponent.hpp>

#include "GameState.hpp"

using namespace sl;

GameState::~GameState()
{
	Locator::world->m_registry.reset();
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