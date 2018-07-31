///
/// GameState.cpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <sl/libs/entt/entity/helper.hpp>

#include <sl/core/World.hpp>
#include <allegro5/keyboard.h>
#include <sl/tags/CameraTag.hpp>
#include <sl/core/ServiceLocator.hpp>
#include <sl/systems/RenderSystem.hpp>
#include <sl/components/RenderComponent.hpp>
#include <sl/components/TransformComponent.hpp>
#include <sl/components/ScrollingBackgroundComponent.hpp>
#include <sl/graphics/Window.hpp>
#include <sl/resources/MusicPlayer.hpp>

#include "GameState.hpp"

using namespace sl;

GameState::GameState()
	:serialize("savefiles/")
{
	//map = std::make_unique<sl::TMXMap>("platformer.tmx", 2.5f);
	//m_bounds.m_height = map->m_internalMap->height * map->m_internalMap->tile_height;
	//m_bounds.m_width = map->m_internalMap->width * map->m_internalMap->tile_width;
	m_bounds.m_height = 0;
	m_bounds.m_width = 0;
	m_bounds.m_x = 0;
	m_bounds.m_y = 0;

	//Locator::musicPlayer->get("background").play();

	entt::DefaultRegistry::entity_type scrolledEntity = Locator::world->m_registry.create();
	Locator::world->m_registry.assign<TransformComponent>(scrolledEntity, 1, 0.0f, Rect<float, int>{ 0.0f, 0.0f, 1280, 720 });
	Locator::world->m_registry.assign<ScrollingBackgroundComponent>(scrolledEntity, 1.0f);
	Locator::world->m_registry.assign<RenderComponent>(scrolledEntity, 1.0f, "bg_forest");
}

GameState::~GameState()
{
	Locator::world->m_registry.reset();
}

void GameState::load()
{
}

void GameState::unload()
{
}

void GameState::event(ALLEGRO_EVENT* event)
{
	switch (event->type)
	{
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (event->keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			Locator::window->close();
			break;

		case ALLEGRO_KEY_S:
			serialize.createGameSnapshot("test.sav", Locator::world->m_registry);
			break;

		case ALLEGRO_KEY_L:
			serialize.loadGameSnapshot("test.sav", Locator::world->m_registry);
			break;
		}
		break;
	}
}

void GameState::update(double dt)
{
}

void GameState::render()
{
	Locator::world->getSystem<RenderSystem>()->render(Locator::world->m_registry);
}