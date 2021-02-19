///
/// Sandbox.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/flags/Enabled.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/graphics/Shader.hpp>

#include "scenes/SandboxScene.hpp"
#include "scenes/PhysicsScene.hpp"
#include "scenes/MapScene.hpp"

#include "Sandbox.hpp"

namespace sb
{
	Sandbox::Sandbox()
	{
		m_window = SL_HANDLE.window();

		m_sandbox_scene = std::make_unique<SandboxScene>();
		m_physics_scene = std::make_unique<PhysicsScene>();
		m_map_scene     = std::make_unique<MapScene>();
		m_active_scene  = m_sandbox_scene.get();
	}

	Sandbox::~Sandbox()
	{
		m_window = nullptr;
	}

	void Sandbox::on_push()
	{
	}

	void Sandbox::on_pop()
	{
	}

	void Sandbox::events()
	{
		if (m_window->key_pressed(galaxy::input::Keys::ESC))
		{
			m_window->close();
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_1))
		{
			m_active_scene = m_sandbox_scene.get();
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_2))
		{
			m_active_scene = m_physics_scene.get();
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_3))
		{
			m_active_scene = m_map_scene.get();
		}

		m_active_scene->events();
	}

	void Sandbox::update(const double dt)
	{
		m_active_scene->update(dt);
	}

	void Sandbox::pre_render()
	{
		m_active_scene->pre_render();
	}

	void Sandbox::render()
	{
		m_active_scene->render();
	}
} // namespace sb