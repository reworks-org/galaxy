///
/// EditorLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "../editor/Theme.hpp"
#include "../resources/Roboto-Light.hpp"

#include "EditorLayer.hpp"

using namespace galaxy;

namespace sc
{
	EditorLayer::EditorLayer()
	{
		GALAXY_LOG_CAPTURE_CUSTOM(m_std_console.get_stream());
		m_window = SL_HANDLE.window();

		m_framebuffer.create(1, 1);
		m_scene_map["Editor"] = std::make_unique<EditorScene>();
		m_active_scene        = m_scene_map["Editor"].get();

		// clang-format off
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;
		io.ConfigDockingWithShift    = true;
		io.IniFilename = "sclayout.ini";

		ImFontConfig font_config = {};
		font_config.FontDataOwnedByAtlas = false;
		font_config.RasterizerMultiply = 1.5f;
		font_config.OversampleH = 4;
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 16.0f, &font_config);
		// clang-format on

		editor::theme::visual_dark();

		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	EditorLayer::~EditorLayer()
	{
		m_active_scene = nullptr;
		m_scene_map.clear();

		m_window = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		GALAXY_LOG_CAPTURE_CUSTOM(std::cout);
	}

	void EditorLayer::on_push()
	{
		m_entity_panel.set_scene(m_active_scene);
		m_camera_panel.set_scene(m_active_scene);
	}

	void EditorLayer::on_pop()
	{
	}

	void EditorLayer::events()
	{
		if (m_viewport_focused && m_viewport_hovered)
		{
			ImGui_ImplGlfw::g_BlockInput = true;
			m_active_scene->events();
		}
		else
		{
			ImGui_ImplGlfw::g_BlockInput = false;
		}

		if (m_window->key_pressed(input::Keys::ESC))
		{
			exit();
		}
	}

	void EditorLayer::update(const double dt)
	{
		m_active_scene->update(dt);
	}

	void EditorLayer::pre_render()
	{
		for (const auto& gl_operation : m_gl_operations)
		{
			gl_operation();
		}

		m_gl_operations.clear();

		m_active_scene->pre_render();

		m_framebuffer.bind();
		m_active_scene->render();
		m_framebuffer.unbind();

		imgui_render();
	}

	void EditorLayer::render()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorLayer::on_event(const events::WindowResized& event)
	{
		m_framebuffer.change_size(event.m_width, event.m_height);
	}

	void EditorLayer::imgui_render()
	{
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
		ImGui::Begin("Dev Tools", NULL, window_flags);
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

		m_camera_panel.render();
		m_entity_panel.render(m_gl_operations);
		m_json_panel.parse_and_display();
		m_console.render();
		m_scene_panel.render(m_scene_map);
		m_script_panel.render();
		m_std_console.render();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		if (ImGui::Begin("Viewport"))
		{
			m_viewport_focused    = ImGui::IsWindowFocused();
			m_viewport_hovered    = ImGui::IsWindowHovered();
			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.change_size(m_viewport_size.x, m_viewport_size.y);
			}

			ImGui::Image(reinterpret_cast<void*>(m_framebuffer.gl_texture()), m_viewport_size, {0, 1}, {1, 0});
		}

		ImGui::End();
		ImGui::PopStyleVar(1);

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}

		ImGui::End();
		end();
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