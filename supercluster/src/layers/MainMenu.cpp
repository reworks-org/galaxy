///
/// MainMenu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileListener.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "../layers/Editor.hpp"

#include "MainMenu.hpp"

using namespace galaxy;

namespace sc
{
	MainMenu::MainMenu() noexcept
		: m_editor {nullptr}
	{
		set_name("Main Menu");

		//m_scene_stack.create("MainMenu");
		//m_scene_stack.push("MainMenu");
	}

	MainMenu::~MainMenu() noexcept
	{
		m_editor = nullptr;

		//m_scene_stack.clear();
	}

	void MainMenu::events()
	{
		//m_scene_stack.events();
	}

	void MainMenu::update()
	{
		//m_scene_stack.update();
	}

	void MainMenu::pre_render()
	{
		ui::imgui_new_frame();

		// clang-format off
		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		// clang-format on

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::SetCursorPos({(ImGui::GetWindowWidth() / 2.0f) - 150, (ImGui::GetWindowHeight() / 2.0f) - 50});

		if (ImGui::Button("New", {100, 50}))
		{
			m_editor->new_project();
			m_app->push_layer(m_editor);
		}

		if (ImGui::Button("Load", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog("*.scproj", "projects/");
			if (file != std::nullopt)
			{
				m_editor->load_project(file.value());
				m_app->push_layer(m_editor);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open selected project file.");
			}
		}

		if (ImGui::Button("Exit", {100, 50}))
		{
			SL_HANDLE.window()->close();
		}

		ImGui::End();

		//m_scene_stack.pre_render();
	}

	void MainMenu::render()
	{
		ui::imgui_render();

		//m_scene_stack.render();
	}
} // namespace sc