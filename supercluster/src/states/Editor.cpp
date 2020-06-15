///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "../layers/ECSLayer.hpp"
#include "../layers/ImGuiLayer.hpp"

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
	{
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::onPush() noexcept
	{
		m_layers.add<ECSLayer>();
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

	void Editor::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		m_layers.update(deltaTime);
	}

	void Editor::render() noexcept
	{
		m_layers.render();
	}
}