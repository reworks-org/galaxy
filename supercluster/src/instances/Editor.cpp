///
/// EditorInstance.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/ServiceLocator.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "editor/Theme.hpp"
#include "resources/Roboto-Light.hpp"

#include "EditorInstance.hpp"

using namespace galaxy;

namespace sc
{
	EditorInstance::EditorInstance()
	{
		m_name = "Editor";

		// clang-format off
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;
		io.ConfigDockingWithShift    = true;
		io.IniFilename = "assets/sclayout.ini";

		ImFontConfig font_config = {};
		font_config.FontDataOwnedByAtlas = false;
		font_config.RasterizerMultiply = 1.5f;
		font_config.OversampleH = 4;
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 16.0f, &font_config);
		// clang-format on

		ImGui_ImplGlfw_InitForOpenGL(SL_HANDLE.window()->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		editor::theme::visual_dark();
		m_menu.enable();
	}

	EditorInstance::~EditorInstance()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		GALAXY_LOG_CAPTURE_CUSTOM(std::cout);
	}

	void EditorInstance::events()
	{
		if (!m_menu.active())
		{
			if (!m_game_mode)
			{
				SL_HANDLE.window()->set_scene_dispatcher(nullptr);
			}
			else
			{
				if (SL_HANDLE.window()->key_pressed(input::Keys::ESC))
				{
					m_game_mode = false;
				}

				SL_HANDLE.window()->set_scene_dispatcher(&m_project.get_top_scene()->m_dispatcher);
			}

			m_editor.events();
		}
	}

	void EditorInstance::update(const double dt)
	{
		if (!m_menu.active())
		{
			m_editor.update();
		}
	}

	void EditorInstance::pre_render()
	{
		if (!m_menu.active())
		{
			start();
			m_editor.pre_render(&m_project, &m_game_mode);
			end();
		}
		else
		{
			start();
			m_menu.pre_render(&m_project);
			end();
		}
	}

	void EditorInstance::render()
	{
		if (!m_game_mode)
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else
		{
			m_editor.render();
		}
	}

	void EditorInstance::start()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorInstance::end()
	{
		ImGui::Render();
	}
} // namespace sc