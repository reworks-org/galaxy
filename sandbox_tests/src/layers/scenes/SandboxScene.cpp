///
/// Sandbox.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>

#include <galaxy/res/MusicBook.hpp>
#include <galaxy/res/SoundBook.hpp>
#include <galaxy/res/TextureAtlas.hpp>

#include <galaxy/systems/RenderSystem.hpp>

#include "SandboxScene.hpp"

using namespace galaxy;
using namespace std::chrono_literals;

namespace sb
{
	SandboxScene::SandboxScene()
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		m_world.create_from_json("point.json");
		m_world.create_from_json("line.json");
		m_world.create_from_json("circle.json");
		m_world.create_from_json("sprite.json");
		m_world.create_from_json("batch_a.json");
		m_world.create_from_json("batch_b.json");
		m_world.create_from_json("text.json");

		m_world.create_system<systems::RenderSystem>();

		m_timer.set_repeating(true);
		m_timer.launch([]() {
			std::cout << "Timer Ping" << std::endl;
		},
			       1000);
	}

	SandboxScene::~SandboxScene()
	{
		// Will block thread until finished.
		// This is intended.
		m_timer.stop();
	}

	void SandboxScene::events()
	{
		if (SL_HANDLE.window()->key_pressed(input::Keys::M))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->play();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::P))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->pause();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::N))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->stop();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::B))
		{
			SL_HANDLE.soundbook()->get("button")->play();
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

	void SandboxScene::update(const double dt)
	{
		m_camera.update(dt);
		m_world.update(dt);
	}

	void SandboxScene::pre_render()
	{
	}

	void SandboxScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
	}
} // namespace sb