///
/// Game.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "Game.hpp"

namespace sc
{
	Game::Game()
	{
		m_name = "Game";
	}

	Game::~Game()
	{
		m_scene_stack.clear();
	}

	void Game::events()
	{
		m_scene_stack.events();
	}

	void Game::update()
	{
		m_scene_stack.update();
	}

	void Game::pre_render()
	{
		m_scene_stack.pre_render();
	}

	void Game::render()
	{
		m_scene_stack.render();
	}
} // namespace sc