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

		m_scene_stack.create<SandboxScene>("SandboxScene");
		m_scene_stack.create<PhysicsScene>("PhysicsScene");
		m_scene_stack.create<MapScene>("MapScene");

		m_scene_stack.push("SandboxScene");
	}

	Sandbox::~Sandbox()
	{
		m_window = nullptr;
	}

	void Sandbox::events()
	{
		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::Z))
		{
			//galaxy::fs::Serializer::serialize(this, "assets/saves/");
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::X))
		{
			//galaxy::fs::Serializer::deserialize(this, "assets/saves/");
		}

		if (m_window->key_pressed(galaxy::input::Keys::ESC))
		{
			m_window->close();
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_1))
		{
			m_scene_stack.pop();
			m_scene_stack.push("SandboxScene");
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_2))
		{
			m_scene_stack.pop();
			m_scene_stack.push("PhysicsScene");
		}

		if (m_window->key_pressed(galaxy::input::Keys::NUM_3))
		{
			m_scene_stack.pop();
			m_scene_stack.push("MapScene");
		}

		m_scene_stack.events();
	}

	void Sandbox::update(const double dt)
	{
		m_scene_stack.update(dt);
	}

	void Sandbox::pre_render()
	{
		m_scene_stack.pre_render();
	}

	void Sandbox::render()
	{
		m_scene_stack.render();
	}
} // namespace sb