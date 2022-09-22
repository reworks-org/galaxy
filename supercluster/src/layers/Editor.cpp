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
#include <galaxy/core/Config.hpp>
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
		m_log_console.set_sink(&sink);

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
		m_world.m_dispatcher.sink<events::MouseWheel>().disconnect();

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

			if (m_project_scenes.has_current())
			{
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

			// m_window->trigger_queued_events(m_world.m_dispatcher);
		}
	}

	void Editor::update()
	{
		if (m_project_scenes.has_current() && !m_paused)
		{
			m_project_scenes.current().update();
		}

		for (const auto& update : m_update_stack)
		{
			update();
		}

		m_update_stack.clear();
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

		if (m_project_scenes.has_current())
		{
			m_project_scenes.current().render();
		}

		graphics::Renderer::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
		glViewport(s_viewport[0], s_viewport[1], s_viewport[2], s_viewport[3]);

		ui::imgui_new_frame();

		if (m_use_mouse_hand)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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

				if (ImGui::MenuItem("Save As"))
				{
					save_project(true);
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
					auto& config = core::ServiceLocator<core::Config>::ref();

					if (ImGui::MenuItem("Classic"))
					{
						ImGui::StyleColorsClassic();

						config.set<std::string>("theme", "CLASSIC", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Light"))
					{
						ImGui::StyleColorsLight();

						config.set<std::string>("theme", "LIGHT", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Dark"))
					{
						ImGui::StyleColorsDark();

						config.set<std::string>("theme", "DARK", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Enhanced Light"))
					{
						ui::imgui_theme_enhanced_light();

						config.set<std::string>("theme", "ENHANCED_LIGHT", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Enhanced Dark"))
					{
						ui::imgui_theme_enhanced_dark();

						config.set<std::string>("theme", "ENHANCED_DARK", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Material Dark"))
					{
						ui::imgui_theme_material_dark();

						config.set<std::string>("theme", "MATERIAL_DARK", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Visual Dark"))
					{
						ui::imgui_theme_visual_dark();

						config.set<std::string>("theme", "VISUAL_DARK", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Fancy Dark"))
					{
						ui::imgui_theme_fancy_dark();

						config.set<std::string>("theme", "FANCY_DARK", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Dark Embrace"))
					{
						ui::imgui_theme_dark_embrace();

						config.set<std::string>("theme", "DARK_EMBRACE", "editor");
						config.save();
					}

					if (ImGui::MenuItem("Gold"))
					{
						ui::imgui_theme_gold();

						config.set<std::string>("theme", "GOLD", "editor");
						config.save();
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
					GALAXY_LOG(GALAXY_INFO, "SHOWING DEBUG WINDOW.");
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
		m_log_console.render();

		// Center:
		m_json_panel.render();
		viewport();

		// Left:
		m_entity_panel.render(m_selected_entity, m_update_stack);

		// Right:
		m_scene_panel.render(m_project_scenes, m_selected_entity);

		ImGui::End();

		ui::imgui_prerender();
		ui::imgui_render();
	}

	void Editor::new_project()
	{
		m_window->set_title("Untitled Project - Supercluster Editor");

		m_project_scenes.clear();
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
				m_project_scenes.deserialize(json.value());

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

	void Editor::save_project(bool save_as)
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		if (m_current_project_path.empty() || save_as)
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
				m_framebuffer.resize(static_cast<int>(m_viewport_size.x), static_cast<int>(m_viewport_size.y));
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

/* void Editor::events()
	{
		if (!m_game_mode)
		{
			//	ImGui::ImplGlfw::g_BlockInput = false;

			if (m_viewport_focused && m_viewport_hovered)
			{





			}
		}
		else
		{
			//ImGui::ImplGlfw::g_BlockInput = true;
			m_scene_stack.top()->on_push();

			if (SL_HANDLE.window()->key_pressed(input::Keys::ESC))
			{
				m_game_mode = false;
				deserialize(m_backup);
				SL_HANDLE.window()->set_cursor_visibility(true);
			}

			m_scene_stack.events();
		}
}
} // namespace sc

if (ImGui::BeginPopupContextItem("RightClickCreateEntityPopup"))
{
if (ImGui::BeginMenu("  Create"))
{
if (ImGui::BeginMenu("  Entity"))
{
if (m_topscene_type == "2D")
{
if (ImGui::MenuItem("  Sprite"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::BatchSprite>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Animated Sprite"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Animated>(entity);
world.create_component<components::BatchSprite>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Primitive2D"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Primitive2D>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::LINE_LOOP;
}

if (ImGui::MenuItem("  Rigid Body"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::BatchSprite>(entity);
world.create_component<components::OnCollision>(entity);
world.create_component<components::RigidBody>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Animated Body"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Animated>(entity);
world.create_component<components::BatchSprite>(entity);
world.create_component<components::OnCollision>(entity);
world.create_component<components::RigidBody>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Text"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Tag>(entity);
world.create_component<components::Text>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::TEXT;
}
}

ImGui::EndMenu();
}

ImGui::EndMenu();
}

ImGui::EndPopup();
}*/