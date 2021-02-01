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

#include "Sandbox.hpp"

namespace sb
{
	Sandbox::Sandbox()
	{
		m_window       = SL_HANDLE.window();
		m_active_scene = std::make_unique<SandboxScene>();
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
		m_active_scene->events();
		if (m_window->key_pressed(galaxy::input::Keys::ESC))
		{
			m_window->close();
		}
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