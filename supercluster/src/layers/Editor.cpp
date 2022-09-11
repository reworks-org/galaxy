///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

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

	void Editor::new_project()
	{
	}

	void Editor::load_project(std::string_view path)
	{
	}

	void Editor::save_project()
	{
	}

	void Editor::exit()
	{
	}

	void Editor::viewport()
	{
	}

	nlohmann::json Editor::serialize()
	{
		return {};
	}

	void Editor::deserialize(const nlohmann::json& json)
	{
	}
} // namespace sc