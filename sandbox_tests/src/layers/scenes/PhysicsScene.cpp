///
/// PhysicsScene.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/OnEvent.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/events/Collision.hpp>
#include <galaxy/systems/CollisionSystem.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include "PhysicsScene.hpp"

using namespace galaxy;

namespace sb
{
	PhysicsScene::PhysicsScene(std::string_view name)
	    : Scene {name, scenes::Types::WORLD}
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		SL_HANDLE.window()->register_on_window_resize(m_camera);

		m_world.create_system<systems::TransformSystem>();
		m_world.create_system<systems::RenderSystem>();
		m_world.create_system<systems::CollisionSystem>(&m_dispatcher);

		m_cube  = m_world.create_from_json("cube.json").value();
		m_floor = m_world.create_from_json("floor.json").value();

		auto* cube_event       = m_world.create_component<components::OnEvent<events::Collision>>(m_cube);
		cube_event->m_on_event = [&](const events::Collision& collision) -> void {
			GALAXY_LOG(GALAXY_WARNING, "Collision between {0} and {1}.", m_world.get<components::Tag>(collision.m_a)->m_tag, m_world.get<components::Tag>(collision.m_b)->m_tag);
		};

		m_dispatcher.subscribe<events::Collision, components::OnEvent<events::Collision>>(*cube_event);
	}

	PhysicsScene::~PhysicsScene()
	{
	}

	void PhysicsScene::on_push()
	{
	}

	void PhysicsScene::on_pop()
	{
	}

	void PhysicsScene::events()
	{
		if (SL_HANDLE.window()->key_down(input::Keys::W))
		{
			m_world.get<components::Transform>(m_cube)->move(0.0f, -10.0f);
		}

		if (SL_HANDLE.window()->key_down(input::Keys::S))
		{
			m_world.get<components::Transform>(m_cube)->move(0.0f, 10.0f);
		}

		if (SL_HANDLE.window()->key_down(input::Keys::A))
		{
			m_world.get<components::Transform>(m_cube)->move(-10.0f, 0.0f);
		}

		if (SL_HANDLE.window()->key_down(input::Keys::D))
		{
			m_world.get<components::Transform>(m_cube)->move(10.0f, 0.0f);
		}

		if (SL_HANDLE.window()->key_down(input::Keys::Q))
		{
			m_world.get<components::Transform>(m_cube)->rotate(-1.0f);
		}

		if (SL_HANDLE.window()->key_down(input::Keys::E))
		{
			m_world.get<components::Transform>(m_cube)->rotate(1.0f);
		}
	}

	void PhysicsScene::update(const double dt)
	{
		m_camera.update(dt);
		m_world.update(dt);
	}

	void PhysicsScene::pre_render()
	{
	}

	void PhysicsScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
	}

	nlohmann::json PhysicsScene::sub_serialize()
	{
		nlohmann::json json = "{}"_json;
		return json;
	}

	void PhysicsScene::sub_deserialize(const nlohmann::json& json)
	{
	}
} // namespace sb