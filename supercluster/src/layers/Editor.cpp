///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <nlohmann/json.hpp>
#include <portable-file-dialogs.h>

#include <galaxy/algorithm/ZLib.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/scripting/JSON.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor(std::string_view name, state::Scene* scene) noexcept
		: Layer {name, scene}
	{
		auto& sink = GALAXY_ADD_SINK(EditorSink);
		m_std_console.set_sink(&sink);

		m_framebuffer.create(1, 1);
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

		if (m_viewport_focused && m_viewport_hovered)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				m_mouse_picked = true;
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
			{
				m_use_mouse_hand = true;
			}
			else
			{
				m_use_mouse_hand = false;
			}

			auto& camera = m_project_scenes.current().get_camera();
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
			{
				m_imgui_mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

				camera.set_pos(m_imgui_mouse_delta.x, m_imgui_mouse_delta.y);

				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
			}

			if (!m_paused)
			{
				m_project_scenes.current().events();
			}
		}
	}

	void Editor::update()
	{
		if (!m_paused)
		{
			m_project_scenes.current().update();
		}

		/*
for (const auto& gl_operation : m_gl_operations)
{
	gl_operation();
}
m_gl_operations.clear();
*/
	}

	void Editor::render()
	{
#ifdef _DEBUG
		static bool s_show_demo = false;
#endif

		static GLint s_cur_fbo = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_cur_fbo);

		static GLint s_viewport[4] = {0, 0, 0, 0};
		glGetIntegerv(GL_VIEWPORT, s_viewport);

		m_framebuffer.bind(true);

		m_project_scenes.current().render();
		graphics::Renderer::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
		glViewport(s_viewport[0], s_viewport[1], s_viewport[2], s_viewport[3]);

		ui::imgui_new_frame();

		if (m_use_mouse_hand)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		else
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}

		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
															   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
															   ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
		static constexpr const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Viewport", NULL, window_flags);
		ImGui::PopStyleVar(3);

		static const ImVec2 size = {0.0f, 0.0f};
		ImGui::DockSpace(ImGui::GetID("Main Viewport Dockspace"), size, dockspace_flags);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("New"))
				{
					ui::imgui_open_confirm("NewConfirmPopup");
				}

				if (ImGui::MenuItem("Load"))
				{
					ui::imgui_open_confirm("LoadConfirmPopup");
				}

				if (ImGui::MenuItem("Save"))
				{
					save_project();
				}

				if (ImGui::MenuItem("Restart"))
				{
					GALAXY_RESTART = true;
					exit();
				}

				if (ImGui::MenuItem("Exit"))
				{
					exit();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editor"))
			{
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
						ui::imgui_theme_enhanced_light();
					}

					if (ImGui::MenuItem("Enhanced Dark"))
					{
						ui::imgui_theme_enhanced_dark();
					}

					if (ImGui::MenuItem("Material Dark"))
					{
						ui::imgui_theme_material_dark();
					}

					if (ImGui::MenuItem("Visual Dark"))
					{
						ui::imgui_theme_visual_dark();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				/*
				if (ImGui::MenuItem("Tiled"))
				{
					m_tiled_process.create("tools/tiled/tiled.exe");
				}

				if (ImGui::MenuItem("Notepad++"))
				{
					m_notepad_process.create("tools/notepad++/notepad++.exe");
				}
				*/

				// clang-format off

                #ifdef _DEBUG
				if (ImGui::MenuItem("Demo"))
				{
					s_show_demo = !s_show_demo;
				}
				#endif
				// clang-format on

				ImGui::EndMenu();
			}

			ImGui::Text("( ? )");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("[RMB] to drag viewport.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
			if (ImGui::ArrowButton("PlaySceneArrowButton", ImGuiDir_Right))
			{
				// m_game_mode = true;
				// m_backup    = serialize();

				// m_window->set_cursor_visibility(false);
			}

			static std::string s_pause_resume = " | | ##PauseSceneButton";
			if (ImGui::Button(s_pause_resume.c_str()))
			{
				if (!m_paused)
				{
					m_paused       = true;
					s_pause_resume = " >> ##ResumeSceneButton";
				}
				else
				{
					m_paused       = false;
					s_pause_resume = " | | ##PauseSceneButton";
				}
			}

			ImGui::EndMenuBar();
		}

		// clang-format off
		ui::imgui_confirm("NewConfirmPopup",
			[&]() {
				save_project();
				new_project();
			},
			[&]() {
				new_project();
			}
		);

		ui::imgui_confirm("LoadConfirmPopup", [&]() {
            auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.scproj");
			if (file.has_value())
			{
				load_project(file.value());
			}
		});

        #ifdef _DEBUG
		if (s_show_demo)
		{
			ImGui::ShowDemoWindow();
		}
        #endif
		// clang-format on

		// Bottom:
		m_lua_console.render();
		m_std_console.render();

		// Center:
		m_json_panel.render();
		viewport();

		// Left:
		// m_entity_panel.render(m_scene_stack.top(), m_gl_operations);

		// Right:
		// m_audio_panel.render();
		m_scene_panel.render(m_project_scenes);

		ImGui::End();

		ui::imgui_prerender();
		ui::imgui_render();

		/*
		if (!m_game_mode)
		{
			m_scene_stack.pre_render();
			imgui_render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else
		{
			m_scene_stack.pre_render();
			m_scene_stack.render();
		}
		*/
	}

	void Editor::new_project()
	{
		m_window->set_title("Untitled Project - Supercluster Editor");

		m_project_scenes.clear();
		m_project_scenes.make("Untitled");
		m_project_scenes.set("Untitled");
	}

	void Editor::load_project(std::string_view path)
	{
		std::ifstream ifs {static_cast<std::string>(path), std::ifstream::in | std::ifstream::binary | std::ifstream::ate};

		if (ifs.good())
		{
			std::vector<char> buffer;

			const auto size = ifs.tellg();
			buffer.resize(size);

			ifs.seekg(0, std::ifstream::beg);
			ifs.read(&buffer[0], size);
			ifs.close();

			const auto fs_path     = std::filesystem::path(path);
			m_current_project_path = fs_path.string();

			auto decompressed = std::string(buffer.begin(), buffer.end());
			auto json         = json::parse_from_mem(decompressed);

			if (json.has_value())
			{
				deserialize(json.value());

				const auto title = std::format("{0} - Supercluster Editor", fs_path.stem().string());
				m_window->set_title(title.c_str());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", fs_path.string());
			}
		}
		else
		{
			ifs.close();
			GALAXY_LOG(GALAXY_ERROR, "Failed to open project file: {0}.", path);
		}
	}

	void Editor::save_project()
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		if (m_current_project_path.empty())
		{
			const auto sp_opt = fs.show_save_dialog("*.scproj", "untitled.scproj");
			if (sp_opt.has_value())
			{
				m_current_project_path = sp_opt.value();
			}
		}

		if (!m_current_project_path.empty())
		{
			std::ofstream ofs {m_current_project_path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};

			if (ofs.good())
			{
				auto data = m_project_scenes.serialize().dump(4);
				ofs.write(data.data(), data.size());
				ofs.close();

				const auto title = std::format("{0} - Supercluster Editor", std::filesystem::path(m_current_project_path).stem().string());
				m_window->set_title(title.c_str());
			}
			else
			{
				ofs.close();
				GALAXY_LOG(GALAXY_ERROR, "Failed to save project to disk.");
			}
		}
	}

	void Editor::exit()
	{
		m_window->close();
	}

	void Editor::viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			m_viewport_focused = ImGui::IsWindowFocused();
			m_viewport_hovered = ImGui::IsWindowHovered();

			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.resize(m_viewport_size.x, m_viewport_size.y);
			}

			ImGui::Image(reinterpret_cast<void*>(m_framebuffer.get_texture()), m_viewport_size, {0, 1}, {1, 0});

			/*
			if (m_mouse_picked)
			{
				auto scene = m_scene_stack.top();

				m_clicked_pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - scene->m_camera.get_pos().x;
				m_clicked_pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - scene->m_camera.get_pos().y;
				m_cursor_aabb.set(m_clicked_pos, {m_clicked_pos + m_cursor_size});

				scene->m_world.operate<components::Renderable>(std::execution::par, [&](const ecs::Entity entity, components::Renderable* renderable) {
					if (renderable->get_aabb().overlaps(m_cursor_aabb, true))
					{
						m_entity_panel.set_selected_entity(std::make_optional(entity));
					}
				});

				m_mouse_picked = false;
			}
			*/

			ImGui::End();
			ImGui::PopStyleVar(1);
		}
	}

	nlohmann::json Editor::serialize()
	{
		static_assert(true, "Do Not Call.");
		return {};
	}

	void Editor::deserialize(const nlohmann::json& json)
	{
		static_assert(true, "Do Not Call.");
	}
} // namespace sc