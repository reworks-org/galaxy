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

#include "SandboxScene.hpp"

namespace sb
{
	SandboxScene::SandboxScene()
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(10.0f);
	}

	SandboxScene::~SandboxScene()
	{
	}

	void SandboxScene::events()
	{
		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::M))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->play();
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::P))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->pause();
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::N))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->stop();
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::B))
		{
			SL_HANDLE.soundbook()->get("button")->play();
		}

		if (SL_HANDLE.window()->key_down(galaxy::input::Keys::W))
		{
			m_camera.on_key_down({galaxy::input::Keys::W});
		}
		else
		{
			m_camera.on_key_up({galaxy::input::Keys::W});
		}

		if (SL_HANDLE.window()->key_down(galaxy::input::Keys::S))
		{
			m_camera.on_key_down({galaxy::input::Keys::S});
		}
		else
		{
			m_camera.on_key_up({galaxy::input::Keys::S});
		}

		if (SL_HANDLE.window()->key_down(galaxy::input::Keys::A))
		{
			m_camera.on_key_down({galaxy::input::Keys::A});
		}
		else
		{
			m_camera.on_key_up({galaxy::input::Keys::A});
		}

		if (SL_HANDLE.window()->key_down(galaxy::input::Keys::D))
		{
			m_camera.on_key_down({galaxy::input::Keys::D});
		}
		else
		{
			m_camera.on_key_up({galaxy::input::Keys::D});
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
	}
} // namespace sb