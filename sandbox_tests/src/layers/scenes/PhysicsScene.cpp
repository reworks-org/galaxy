///
/// PhysicsScene.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/OnEvent.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/events/Collision.hpp>
#include <galaxy/events/FinishCollision.hpp>
#include <galaxy/systems/PhysicsSystem.hpp>
#include <galaxy/systems/RenderSystem.hpp>

#include "PhysicsScene.hpp"

using namespace galaxy;

namespace sb
{
	PhysicsScene::PhysicsScene()
	    : m_contact_listener {&m_dispatcher}
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		SL_HANDLE.window()->register_on_window_resize(m_camera);

		m_world.create_system<systems::PhysicsSystem>();
		m_world.create_system<systems::RenderSystem>();

		m_world.set_gravity(0.0f, 1.0f);
		m_world.b2_world()->SetContactListener(&m_contact_listener);

		auto floor = m_world.create_from_json("floor.json").value();
		auto cube  = m_world.create_from_json("cube.json").value();

		auto* floor_event = m_world.create_component<components::OnEvent<events::Collision>>(floor);
		auto* cube_event  = m_world.create_component<components::OnEvent<events::FinishCollision>>(cube);

		floor_event->m_on_event = [&](const events::Collision& collision) -> void {
			GALAXY_LOG(GALAXY_INFO, "Touched floor.");
		};

		cube_event->m_on_event = [&](const events::FinishCollision& finish_collision) -> void {
			GALAXY_LOG(GALAXY_INFO, "Bounced.");
		};

		m_dispatcher.subscribe<events::Collision, components::OnEvent<events::Collision>>(*floor_event);
		m_dispatcher.subscribe<events::FinishCollision, components::OnEvent<events::FinishCollision>>(*cube_event);
	}

	PhysicsScene::~PhysicsScene()
	{
	}

	void PhysicsScene::events()
	{
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
} // namespace sb