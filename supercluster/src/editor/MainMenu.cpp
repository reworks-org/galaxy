///
/// MainMenu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <imgui/imgui_stdlib.h>

#include "MainMenu.hpp"

namespace sc
{
	void MainMenu::render(Project* project)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
		ImGuiViewport* imgui_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(imgui_viewport->GetWorkPos());
		ImGui::SetNextWindowSize(imgui_viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(imgui_viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("MainMenu", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::SetCursorPos({ImGui::GetWindowSize().x / 2.0f - 200, ImGui::GetWindowSize().y / 2.0f});

		if (ImGui::Button("New", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_save_dialog();

			if (file != std::nullopt)
			{
				SL_HANDLE.vfs()->create_file(file.value());

				project->new_project(file.value());

				project->m_game_instance->get_stack().create("Base");
				project->m_game_instance->get_stack().push("Base");

				disable();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create new project file.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Open", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog();
			if (file != std::nullopt)
			{
				project->new_project(file.value());
				project->load(file.value());

				disable();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit", {100, 50}))
		{
			SL_HANDLE.window()->close();
		}

		ImGui::End();
	}

	void MainMenu::enable() noexcept
	{
		main_menu_active = true;
	}

	void MainMenu::disable() noexcept
	{
		main_menu_active = false;
	}

	const bool MainMenu::active() const noexcept
	{
		return main_menu_active;
	}
} // namespace sc