///
/// EditorScene.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <galaxy/core/ServiceLocator.hpp>

#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include "EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	EditorScene::EditorScene(std::string_view name)
	    : Scene {name, scenes::Types::WORLD}
	{
		m_window = SL_HANDLE.window();
		m_camera.create(0.0f, 1024, 1024, 0.0f);
		m_camera.set_speed(100.0f);

		m_world.create_from_json("batch.json");

		m_world.create_system<systems::TransformSystem>();
		m_world.create_system<systems::RenderSystem>();
		m_world.create_system<systems::CollisionSystem>(nullptr);
		m_collision_system = m_world.get_system<systems::CollisionSystem>();
	}

	EditorScene::~EditorScene()
	{
	}

	void EditorScene::on_push()
	{
	}

	void EditorScene::on_pop()
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

	systems::CollisionSystem* EditorScene::get_collision_system() noexcept
	{
		return m_collision_system;
	}

	nlohmann::json EditorScene::sub_serialize()
	{
		nlohmann::json json = "{}"_json;
		return json;
	}

	void EditorScene::sub_deserialize(const nlohmann::json& json)
	{
	}
} // namespace sc