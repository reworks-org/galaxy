///
/// EditorScene.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/systems/RenderSystem.hpp>

#include "EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	EditorScene::EditorScene()
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		SL_HANDLE.window()->register_on_window_resize(m_camera);
		SL_HANDLE.window()->register_on_scroll(m_camera);

		m_world.create_system<systems::RenderSystem>();
	}

	EditorScene::~EditorScene()
	{
	}

	void EditorScene::events()
	{
	}

	void EditorScene::update(const double dt)
	{
		m_camera.update(dt);
		m_world.update(dt);
	}

	void EditorScene::pre_render()
	{
	}

	void EditorScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
	}

} // namespace sc