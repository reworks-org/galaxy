///
/// MapScene.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include "MapScene.hpp"

using namespace galaxy;

namespace sb
{
	MapScene::MapScene()
	{
		m_window = SL_HANDLE.window();

		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);
		SL_HANDLE.window()->register_on_window_resize(m_camera);

		m_world.create_system<systems::TransformSystem>();
		m_world.create_system<systems::RenderSystem>();

		m_map.load("assets/maps/desert.json");
		m_map.parse();
		m_map.generate_object_entities(m_world);
	}

	MapScene::~MapScene()
	{
	}

	void MapScene::events()
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

	void MapScene::update(const double dt)
	{
		m_camera.update(dt);
		m_world.update(dt);
	}

	void MapScene::pre_render()
	{
	}

	void MapScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
	}
} // namespace sb