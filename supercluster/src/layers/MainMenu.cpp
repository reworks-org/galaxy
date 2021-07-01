///
/// MainMenu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "MainMenu.hpp"

using namespace galaxy;

namespace sc
{
	MainMenu::MainMenu(core::Application* app) noexcept
	    : Layer {app}
	{
		m_name = "Main Menu";
		//m_scene_stack.create("MainMenu");
		//m_scene_stack.push("MainMenu");
	}

	MainMenu::~MainMenu() noexcept
	{
		//m_scene_stack.clear();
		m_app = nullptr;
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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
		ImGuiViewport* imgui_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
		ImGui::SetNextWindowSize(imgui_viewport->WorkSize);
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
			//m_editor->new_project();
			//m_app->set_instance(m_editor);
		}

		ImGui::SameLine();

		if (ImGui::Button("Open", {100, 50}))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog("*.scproj");
			if (file != std::nullopt)
			{
				//m_editor->load(file.value());
				//m_app->set_instance(m_editor);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit", {100, 50}))
		{
			//m_editor->exit();
		}

		ImGui::End();
		ImGui::Render();
	}

	void MainMenu::render()
	{
		//m_scene_stack.render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
} // namespace sc