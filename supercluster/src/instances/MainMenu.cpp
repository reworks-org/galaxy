///
/// MainMenu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <imgui/imgui_stdlib.h>

#include "MainMenu.hpp"

using namespace galaxy;

namespace sc
{
	MainMenu::MainMenu(core::Application* app, std::shared_ptr<Editor> editor, Project* project) noexcept
	    : m_app {app}, m_editor {editor}, m_project {project}
	{
		m_name = "Main Menu";
	}

	MainMenu::~MainMenu() noexcept
	{
		m_scene_stack.clear();
		m_app     = nullptr;
		m_project = nullptr;
	}

	void MainMenu::events()
	{
		//m_scene_stack.events();
	}

	void MainMenu::update(const double dt)
	{
		//m_scene_stack.update(dt);
	}

	void MainMenu::pre_render()
	{
		//m_scene_stack.pre_render();
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

		ImGui::SetCursorPos({(ImGui::GetWindowWidth() / 2.0f) - 150, (ImGui::GetWindowHeight() / 2.0f) - 50});
		if (ImGui::Button("New", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_save_dialog();

			if (file != std::nullopt)
			{
				SL_HANDLE.vfs()->create_file(file.value());
				m_project->new_project(file.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create new project file.");
			}

			m_app->set_instance(m_editor);
		}

		ImGui::SameLine();

		if (ImGui::Button("Open", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog();
			if (file != std::nullopt)
			{
				m_project->new_project(file.value());
				m_project->load(file.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
			}

			m_app->set_instance(m_editor);
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit", {100, 50}))
		{
			SL_HANDLE.window()->close();
		}

		ImGui::End();
	}

	void MainMenu::render()
	{
		//m_scene_stack.render();
	}
} // namespace sc