///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor(std::string_view name, state::Scene* scene) noexcept
		: Layer {name, scene}
	{
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::on_push()
	{
	}

	void Editor::on_pop()
	{
	}

	void Editor::events()
	{
		// m_window->trigger_queued_events(m_world.m_dispatcher);
	}

	void Editor::update()
	{
		// m_world.update_systems(this);
	}

	void Editor::render()
	{
	}
} // namespace sc