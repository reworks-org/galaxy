///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <iostream>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>
#include <galaxy/scripting/JSONUtils.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "editor/Theme.hpp"
#include "resources/Roboto-Light.hpp"

#include "Editor.hpp"

using namespace galaxy;

namespace sc
{
	Editor::Editor()
	{
		GALAXY_LOG_CAPTURE_CUSTOM(m_std_console.get_stream());
		m_window = SL_HANDLE.window();

		m_framebuffer.create(1, 1);
		m_scene_stack.create<EditorScene>("EditorScene");
		m_scene_stack.push("EditorScene");

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

		editor::theme::visual_dark();

		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	Editor::~Editor()
	{
		m_window = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		GALAXY_LOG_CAPTURE_CUSTOM(std::cout);
	}

	void Editor::events()
	{
		if (!m_first_start)
		{
			if (!m_game_mode)
			{
				if (m_viewport_focused && m_viewport_hovered)
				{
					ImGui_ImplGlfw::g_BlockInput = true;

					m_mouse_dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Right);

					if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					{
						m_mouse_picked = true;
					}
				}
				else
				{
					m_mouse_dragging             = false;
					ImGui_ImplGlfw::g_BlockInput = false;
				}

				if (!ImGui_ImplGlfw::g_BlockInput)
				{
					// Editor hotkeys.
				}

				if (m_window->key_pressed(input::Keys::ESC))
				{
					exit();
				}

				m_scene_stack.events();
			}
			else
			{
				m_project->m_instance->get_stack().events();
			}
		}
	}

	void Editor::update(const double dt)
	{
		if (!m_first_start)
		{
			if (!m_game_mode)
			{
				m_scene_stack.update(dt);
			}
			else
			{
				m_project->m_instance->get_stack().update(dt);
			}
		}
	}

	void Editor::pre_render()
	{
		if (m_first_start)
		{
			first_start();
		}
		else
		{
			if (!m_game_mode)
			{
				for (const auto& gl_operation : m_gl_operations)
				{
					gl_operation();
				}

				m_gl_operations.clear();

				m_scene_stack.pre_render();

				m_framebuffer.bind();
				m_scene_stack.render();
				m_framebuffer.unbind();

				imgui_render();
			}
			else
			{
				m_project->m_instance->get_stack().pre_render();
			}
		}
	}

	void Editor::render()
	{
		if (!m_game_mode)
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else
		{
			m_project->m_instance->get_stack().render();
		}
	}

	void Editor::on_event(const events::WindowResized& event)
	{
		m_framebuffer.change_size(event.m_width, event.m_height);
	}

	void Editor::imgui_render()
	{
		start();

		ImGuiWindowFlags window_flags      = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiViewport* imgui_viewport      = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(imgui_viewport->GetWorkPos());
		ImGui::SetNextWindowSize(imgui_viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(imgui_viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Supercluster", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("EditorScene_Dockspace_1"), {0.0f, 0.0f}, dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("New"))
				{
					auto file = SL_HANDLE.vfs()->show_save_dialog();

					if (file != std::nullopt)
					{
						SL_HANDLE.vfs()->create_file(file.value());

						save_project();
						new_project(file.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to create new project file.");
					}
				}

				if (ImGui::MenuItem("Load"))
				{
					auto file = SL_HANDLE.vfs()->show_open_dialog();
					if (file != std::nullopt)
					{
						save_project();
						new_project(file.value());

						auto json = json::parse_from_disk(file.value());
						if (json != std::nullopt)
						{
							m_project->deserialize(json.value());
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to open json for project.");
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					save_project();
				}

				if (ImGui::MenuItem("Mount Folder"))
				{
					const auto& folder = SL_HANDLE.vfs()->show_folder_dialog();
					if (folder != std::nullopt)
					{
						if (!SL_HANDLE.vfs()->mount(folder.value()))
						{
							GALAXY_LOG(GALAXY_WARNING, "Attempted to mount invalid folder.");
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Attempted to open invalid folder.");
					}
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

				if (ImGui::MenuItem("Show ImGui::Demo"))
				{
					m_render_demo = !m_render_demo;
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

			ImGui::Text("( ? )");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("[SHIFT] for docking.\n[RMB] to drag viewport.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Play Game", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::ArrowButton("PlaySceneArrowButton", ImGuiDir_Right))
		{
			m_game_mode = true;
		}

		ImGui::SameLine();

		if (ImGui::Button(" || ##PauseSceneButton"))
		{
			m_game_mode = false;
		}

		ImGui::End();

		m_camera_panel.render();
		m_entity_panel.render(m_gl_operations);
		m_json_panel.parse_and_display();
		m_console.render();
		//m_scene_panel.render(m_project->m_instance->get_stack());
		m_script_panel.render();
		m_std_console.render();
		m_audio_panel.render();

		viewport();

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}

		ImGui::End();
		end();
	}

	void Editor::start()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::end()
	{
		ImGui::Render();
	}

	void Editor::exit()
	{
		platform::close_process(m_process);
		m_window->close();
	}

	void Editor::viewport()
	{
		/*
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoBackground))
		{
			m_viewport_focused    = ImGui::IsWindowFocused();
			m_viewport_hovered    = ImGui::IsWindowHovered();
			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.change_size(m_viewport_size.x, m_viewport_size.y);
				m_active_scene->camera().set_width(m_viewport_size.x);
				m_active_scene->camera().set_height(m_viewport_size.y);
			}

			if (m_mouse_picked)
			{
				const constexpr static auto mp_id = std::numeric_limits<ecs::Entity>::max();

				glm::vec2 pos;
				pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - m_active_scene->camera().get_pos().x;
				pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - m_active_scene->camera().get_pos().y;

				auto* tree = m_editor_scene->get_collision_system()->get_tree();
				tree->insert(mp_id, {pos.x, pos.y}, {pos.x + 4, pos.y + 4});

				// Will be erased by collision system, as this is after update().
				std::vector<ecs::Entity> entity;
				tree->query(mp_id, std::back_inserter(entity));

				if (entity.size() > 0)
				{
					m_entity_panel.set_selected_entity(std::make_optional(entity[0]));
				}
				else
				{
					m_entity_panel.set_selected_entity(std::nullopt);
				}

				m_mouse_picked = false;
			}

			if (m_mouse_dragging)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_None);
				const auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

				m_active_scene->camera().move(delta.x, delta.y);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
			}
			else
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			}

			ImGui::Image(reinterpret_cast<void*>(m_framebuffer.gl_texture()), m_viewport_size, {0, 1}, {1, 0});
		}

		ImGui::End();
		ImGui::PopStyleVar(1);
		*/
	}

	void Editor::save_project()
	{
		if (m_project)
		{
			json::save_to_disk(m_project->get_path(), m_project->serialize());
		}
	}

	void Editor::new_project(std::string_view path)
	{
		// Implictly destroys existing project.
		m_project             = nullptr;
		m_project             = std::make_unique<Project>(path);
		m_project->m_instance = std::make_shared<Game>();

		m_project->m_instance->get_stack().create<scenes::WorldScene>("Default Scene");
		m_project->m_instance->get_stack().push("Default Scene");

		m_camera_panel.set_instance(m_project->m_instance.get());
		m_entity_panel.set_instance(m_project->m_instance.get());
	}

	void Editor::first_start()
	{
		start();

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
		ImGui::Begin("Supercluster", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::SetCursorPos({ImGui::GetWindowSize().x / 2.0f, ImGui::GetWindowSize().y / 2.0f});

		if (ImGui::Button("New"))
		{
			auto file = SL_HANDLE.vfs()->show_save_dialog();

			if (file != std::nullopt)
			{
				SL_HANDLE.vfs()->create_file(file.value());

				save_project();
				new_project(file.value());

				m_first_start = false;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create new project file.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Open"))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog();
			if (file != std::nullopt)
			{
				save_project();
				new_project(file.value());

				auto json = json::parse_from_disk(file.value());
				if (json != std::nullopt)
				{
					m_project->deserialize(json.value());
					m_first_start = false;
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open json for project.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit"))
		{
			exit();
		}

		ImGui::End();
		end();
	}
} // namespace sc