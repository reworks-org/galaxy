///
/// EditorLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "../editor/Theme.hpp"

#include "EditorLayer.hpp"

using namespace galaxy;

namespace sc
{
	EditorLayer::EditorLayer()
	{
		m_window       = SL_HANDLE.window();
		m_editor_scene = std::make_unique<EditorScene>();
		m_active_scene = m_editor_scene.get();

		// clang-format off
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;
		io.ConfigDockingWithShift    = true;

		const auto default_font_path = SL_HANDLE.vfs()->absolute("Roboto-Regular.ttf");
		io.FontDefault = io.Fonts->AddFontFromFileTTF(default_font_path.c_str(), 16.0f);
		// clang-format on

		editor::theme::visual_dark();
		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);

		static const constexpr char* gl_version = "#version 450 core";
		ImGui_ImplOpenGL3_Init(gl_version);
	}

	EditorLayer::~EditorLayer()
	{
		platform::close_process(m_process);

		m_window  = nullptr;
		m_process = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::on_push()
	{
		m_entity_panel.set_scene(m_active_scene);
	}

	void EditorLayer::on_pop()
	{
	}

	void EditorLayer::events()
	{
		if (m_window->key_pressed(input::Keys::ESC))
		{
			exit();
		}

		m_active_scene->events();
	}

	void EditorLayer::update(const double dt)
	{
		m_active_scene->update(dt);
	}

	void EditorLayer::pre_render()
	{
		m_active_scene->pre_render();
		m_entity_panel.pre_render();

		start();

		ImGuiWindowFlags window_flags      = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiViewport* viewport            = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		static bool s_editor_open = true;
		ImGui::Begin("Dev Tools", &s_editor_open, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("EditorScene_Dockspace_01"), {0.0f, 0.0f}, dockspace_flags);

		ImGui::SameLine();

		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("[INFO]");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("[SHIFT] for docking.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Show ImGui::Demo"))
				{
					m_render_demo = !m_render_demo;
				}

				if (ImGui::BeginMenu("Theme"))
				{
					if (ImGui::MenuItem("Light"))
					{
						ImGui::StyleColorsLight();
					}

					if (ImGui::MenuItem("Dark"))
					{
						ImGui::StyleColorsDark();
					}

					if (ImGui::MenuItem("Classic"))
					{
						ImGui::StyleColorsClassic();
					}

					if (ImGui::MenuItem("Enhanced Light"))
					{
						editor::theme::enhanced_light();
					}

					if (ImGui::MenuItem("Enhanced Dark"))
					{
						editor::theme::enhanced_dark();
					}

					if (ImGui::MenuItem("Material Dark"))
					{
						editor::theme::material_dark();
					}

					if (ImGui::MenuItem("Visual Dark"))
					{
						editor::theme::visual_dark();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Reload"))
				{
					SL_HANDLE.m_restart = true;
					exit();
				}

				if (ImGui::MenuItem("Exit"))
				{
					exit();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Script Editor"))
			{
				m_render_script_editor = !m_render_script_editor;
			}

			if (ImGui::MenuItem("Open Tiled"))
			{
				m_process = platform::run_process("tools/tiled/tiled.exe");
			}

			if (ImGui::MenuItem("Reload"))
			{
				SL_HANDLE.m_restart = true;
				exit();
			}

			ImGui::EndMenuBar();
		}

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}

		if (ImGui::Begin("Viewport"))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			if (ImGui::ArrowButton("PlayArrowButton", ImGuiDir_Right))
			{
			}

			ImGui::SameLine();
			ImGui::Text("Play Scene");

			if (ImGui::Button("||"))
			{
			}

			ImGui::SameLine();
			ImGui::Text("Pause Scene");
		}

		ImGui::End();

		m_entity_panel.render();
		m_json_panel.parse_and_display();
		m_console.render();
		m_scene_panel.render();
		m_script_panel.render(&m_render_script_editor);

		ImGui::End();
		end();
	}

	void EditorLayer::render()
	{
		m_active_scene->render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorLayer::start()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorLayer::end()
	{
		ImGui::Render();
	}

	void EditorLayer::exit()
	{
		platform::close_process(m_process);
		m_window->close();
	}
} // namespace sc