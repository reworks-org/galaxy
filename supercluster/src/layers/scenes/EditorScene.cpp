///
/// EditorScene.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/CollisionSystem.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include "EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	EditorScene::EditorScene()
	    : Scene {"EditorScene"}
	{
		m_window = SL_HANDLE.window();
		m_camera.create(0.0f, 1024, 1024, 0.0f);
		m_camera.set_speed(100.0f);

		m_world.create_from_json("batch.json");

		m_world.create_system<systems::TransformSystem>();
		m_world.create_system<systems::RenderSystem>();
		m_world.create_system<systems::CollisionSystem>(nullptr);
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

		if (SL_HANDLE.window()->key_down(input::Keys::W))
		{
			m_camera.on_key_down({input::Keys::W});
		}
		else
		{
			m_camera.on_key_up({input::Keys::W});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::S))
		{
			m_camera.on_key_down({input::Keys::S});
		}
		else
		{
			m_camera.on_key_up({input::Keys::S});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::A))
		{
			m_camera.on_key_down({input::Keys::A});
		}
		else
		{
			m_camera.on_key_up({input::Keys::A});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::D))
		{
			m_camera.on_key_down({input::Keys::D});
		}
		else
		{
			m_camera.on_key_up({input::Keys::D});
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