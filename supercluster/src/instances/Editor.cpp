///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/algorithm/Algorithm.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>
#include <galaxy/scripting/JSONUtils.hpp>
#include <galaxy/systems/CollisionSystem.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>
#include <portable-file-dialogs.h>

#include "editor/Theme.hpp"
#include "resources/Roboto-Light.hpp"

#include "Editor.hpp"

namespace sc
{
	Editor::Editor()
	{
		m_name = "Editor";
		GALAXY_LOG_CAPTURE_CUSTOM(m_std_console.get_stream());

		// clang-format off
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;
		io.ConfigDockingWithShift = true;
		io.IniFilename = "assets/sclayout.ini";

		ImFontConfig font_config = {};
		font_config.FontDataOwnedByAtlas = false;
		font_config.RasterizerMultiply = 1.5f;
		font_config.OversampleH = 4;
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 16.0f, &font_config);
		// clang-format on

		ImGui_ImplGlfw_InitForOpenGL(SL_HANDLE.window()->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		m_framebuffer.create(1, 1);
		editor::theme::visual_dark();
		SL_HANDLE.window()->set_scene_dispatcher(nullptr);
		m_entity_panel.set_instance(this);
	}

	Editor::~Editor()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		GALAXY_LOG_CAPTURE_CUSTOM(std::cout);
		m_scene_stack.clear();
	}

	void Editor::events()
	{
		if (!m_game_mode)
		{
			SL_HANDLE.window()->set_scene_dispatcher(nullptr);
			ImGui::ImplGlfw::g_BlockInput = false;

			if (m_viewport_focused && m_viewport_hovered)
			{
				m_mouse_dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Right);

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_mouse_picked = true;
				}
			}
			else
			{
				m_mouse_dragging = false;
			}
		}
		else
		{
			ImGui::ImplGlfw::g_BlockInput = true;
			SL_HANDLE.window()->set_scene_dispatcher(&m_scene_stack.top()->m_dispatcher);

			if (SL_HANDLE.window()->key_pressed(input::Keys::ESC))
			{
				m_game_mode = false;
				deserialize(m_backup);
			}

			m_scene_stack.events();
		}
	}

	void Editor::update(const double dt)
	{
		m_scene_stack.update(dt);
	}

	void Editor::pre_render()
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
			m_scene_stack.pre_render();
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
			m_scene_stack.render();
		}
	}

	void Editor::new_project()
	{
		m_scene_stack.clear();
		m_scene_stack.create("Base");
		m_scene_stack.push("Base");
	}

	void Editor::load(std::string_view path)
	{
		const auto compressed = SL_HANDLE.vfs()->open_binary(path);
		if (compressed != std::nullopt)
		{
			const auto size = compressed.value().size();
			m_path          = path;

			auto decompressed = algorithm::decode_zlib({compressed.value().begin(), compressed.value().end()});
			const auto json   = json::parse_from_mem(decompressed);
			if (json != std::nullopt)
			{
				deserialize(json.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", path);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
		}
	}

	void Editor::save()
	{
		auto file = m_path;
		if (file.empty())
		{
			auto opt = SL_HANDLE.vfs()->show_save_dialog("*.scproj");
			if (opt != std::nullopt)
			{
				file = opt.value();
				SL_HANDLE.vfs()->create_file(file);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create new project file.");
			}
		}

		auto compressed = algorithm::encode_zlib(serialize().dump(4));
		SL_HANDLE.vfs()->save_binary(compressed, file);
	}

	void Editor::exit()
	{
		platform::close_process(m_process);
		SL_HANDLE.window()->close();
	}

	void Editor::imgui_render()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags      = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiViewport* imgui_viewport      = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
		ImGui::SetNextWindowSize(imgui_viewport->WorkSize);
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
					pfd::message msg("WARNING", "Save?\nUnsaved work will be lost.", pfd::choice::yes_no, pfd::icon::warning);
					if (msg.result() == pfd::button::yes)
					{
						save();
					}

					new_project();
				}

				if (ImGui::MenuItem("Load"))
				{
					pfd::message msg("WARNING", "Save?\nUnsaved work will be lost.", pfd::choice::yes_no, pfd::icon::warning);
					if (msg.result() == pfd::button::yes)
					{
						save();
					}

					auto file = SL_HANDLE.vfs()->show_open_dialog("*.scproj");
					if (file != std::nullopt)
					{
						load(file.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					save();
				}

				if (ImGui::MenuItem("Restart"))
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

				if (ImGui::MenuItem("Open Tiled"))
				{
					m_process = platform::run_process("tools/tiled/tiled.exe");
				}

				if (ImGui::MenuItem("Show ImGui::Demo"))
				{
					m_render_demo = !m_render_demo;
				}

				ImGui::EndMenu();
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
				m_game_mode = true;
				m_backup    = serialize();

				m_scene_stack.top()->m_camera.set_width(SL_HANDLE.window()->get_width());
				m_scene_stack.top()->m_camera.set_height(SL_HANDLE.window()->get_height());
			}

			if (ImGui::Button(" || ##PauseSceneButton"))
			{
			}

			ImGui::EndMenuBar();
		}

		m_entity_panel.render(m_gl_operations);
		m_json_panel.parse_and_display();
		m_console.render();
		m_scene_panel.render(m_scene_stack);
		m_script_panel.render();
		m_std_console.render();
		m_audio_panel.render();

		viewport();

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}

		ImGui::End();
		ImGui::Render();
	}

	void Editor::viewport()
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

				m_scene_stack.top()->m_camera.set_width(m_viewport_size.x);
				m_scene_stack.top()->m_camera.set_height(m_viewport_size.y);
			}

			if (m_mouse_picked)
			{
				static constexpr const auto mp_id = std::numeric_limits<ecs::Entity>::max();

				glm::vec2 pos;
				pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - m_scene_stack.top()->m_camera.get_pos().x;
				pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - m_scene_stack.top()->m_camera.get_pos().y;

				auto* tree = m_scene_stack.top()->m_world.get_system<systems::CollisionSystem>()->get_tree();
				tree->insert(mp_id, {pos.x, pos.y}, {pos.x + 4, pos.y + 4});

				// Will be erased by collision system, as this is after update().
				static std::vector<ecs::Entity> possible = {};
				tree->query(mp_id, std::back_inserter(possible));

				if (possible.size() > 0)
				{
					m_entity_panel.set_selected_entity(std::make_optional(possible[0]));
				}
				else
				{
					m_entity_panel.set_selected_entity(std::nullopt);
				}

				possible.clear();
				m_mouse_picked = false;
			}

			if (m_mouse_dragging)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_None);
				const auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

				m_scene_stack.top()->m_camera.move(delta.x, delta.y);
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