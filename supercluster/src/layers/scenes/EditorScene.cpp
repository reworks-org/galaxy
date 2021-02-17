///
/// EditorScene.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/systems/RenderSystem.hpp>

#include "EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	EditorScene::EditorScene()
	{
		m_window = SL_HANDLE.window();
		m_camera.create(0.0f, 1024, 1024, 0.0f);
		m_camera.set_speed(100.0f);

		m_world.create_from_json("sprite.json");
		m_world.create_system<systems::RenderSystem>();
	}

	EditorScene::~EditorScene()
	{
	}

	void EditorScene::events()
	{
		if (m_window->is_scrolling())
		{
			m_camera.on_scroll(m_window->get_scroll_data());
		}
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