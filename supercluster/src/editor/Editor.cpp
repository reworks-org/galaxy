///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>
#include <galaxy/systems/CollisionSystem.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_stdlib.h>

#include "Theme.hpp"

#include "Editor.hpp"

namespace sc
{
	Editor::Editor()
	{
		GALAXY_LOG_CAPTURE_CUSTOM(m_std_console.get_stream());
		m_framebuffer.create(1, 1);

		SL_HANDLE.window()->set_scene_dispatcher(nullptr);
	}

	void Editor::events()
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
	}

	void Editor::pre_render(Project* project)
	{
		for (const auto& gl_operation : m_gl_operations)
		{
			gl_operation();
		}

		m_gl_operations.clear();

		project->m_game_instance->get_stack().pre_render();

		m_framebuffer.bind();
		project->m_game_instance->get_stack().render();
		m_framebuffer.unbind();
	}

	void Editor::render(Project* project, bool* game_mode)
	{
		m_entity_panel.set_instance(project->m_game_instance.get());

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

						project->save();
						project->new_project(file.value());
						project->m_game_instance->get_stack().create("Base");
						project->m_game_instance->get_stack().push("Base");
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
						project->save();
						project->new_project(file.value());
						project->load(file.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					project->save();
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

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
			if (ImGui::ArrowButton("PlaySceneArrowButton", ImGuiDir_Right))
			{
				*game_mode = true;
			}

			if (ImGui::Button(" || ##PauseSceneButton"))
			{
			}

			ImGui::EndMenuBar();
		}

		m_entity_panel.render(m_gl_operations);
		m_json_panel.parse_and_display();
		m_console.render();
		m_scene_panel.render(project->m_game_instance->get_stack());
		m_script_panel.render();
		m_std_console.render();
		m_audio_panel.render();

		viewport(project);

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}

		ImGui::End();
	}

	void Editor::exit()
	{
		platform::close_process(m_process);
		SL_HANDLE.window()->close();
	}

	void Editor::viewport(Project* project)
	{
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
				project->get_top_scene()->m_camera.set_width(m_viewport_size.x);
				project->get_top_scene()->m_camera.set_height(m_viewport_size.y);
			}

			if (m_mouse_picked)
			{
				const constexpr static auto mp_id = std::numeric_limits<ecs::Entity>::max();

				glm::vec2 pos;
				pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - project->get_top_scene()->m_camera.get_pos().x;
				pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - project->get_top_scene()->m_camera.get_pos().y;

				auto* tree = project->get_top_scene()->m_world.get_system<systems::CollisionSystem>()->get_tree();
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

				project->get_top_scene()->m_camera.move(delta.x, delta.y);
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
	}
} // namespace sc