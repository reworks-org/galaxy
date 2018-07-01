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
#include <sl/resources/MusicPlayer.hpp>

#include "GameState.hpp"

using namespace sl;

GameState::GameState()
{
	map = std::make_unique<sl::TMXMap>("platformer.tmx", 2.5f);
	m_bounds.m_height = map->m_internalMap->height;
	m_bounds.m_width = map->m_internalMap->width;
	m_bounds.m_x = 0;
	m_bounds.m_y = 0;

	Locator::musicPlayer->get("background").play();
}

GameState::~GameState()
{
	Locator::world->m_registry.reset();
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