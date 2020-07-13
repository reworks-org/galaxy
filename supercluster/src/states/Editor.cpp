///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Window.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "../layers/ECSLayer.hpp"
#include "../layers/GUILayer.hpp"
#include "../layers/ImGuiLayer.hpp"

#include "Editor.hpp"

namespace sc
{
	Editor::Editor(pr::ProtectedDouble* dt) noexcept
		:m_dt(dt)
	{
		m_camera.create(0.0f, SL_HANDLE.window()->getWidth(), SL_HANDLE.window()->getHeight(), 0.0f);
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::onPush() noexcept
	{
		m_layers.add<ECSLayer>();
		m_layers.add<GUILayer>(m_dt);
		m_layers.add<ImGuiLayer>();
	}

	void Editor::onPop() noexcept
	{
		m_layers.clear();
	}

	void Editor::events() noexcept
	{
		m_layers.events();
	}

	void Editor::update(pr::ProtectedDouble* dt) noexcept
	{
		m_camera.update(dt->get());
		m_layers.update(dt);
	}

	void Editor::render() noexcept
	{
		m_layers.render(m_camera);
	}
}