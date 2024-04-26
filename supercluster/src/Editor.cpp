///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imnotify/ImGuiNotify.hpp>

#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/input/Input.hpp>
#include <galaxy/math/Base64.hpp>
#include <galaxy/math/ZLib.hpp>
#include <galaxy/scripting/JSON.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>
#include <galaxy/ui/NuklearUI.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor(const std::string& name)
		: Scene {name}
		, m_state {EditorState::EDITOR}
		, m_padding {4, 4}
	{
		// Reset our editor dispatcher.
		// We dont want anything bound.
		m_dispatcher           = {};
		m_current_project_path = "";

		auto& sink = GALAXY_ADD_SINK(EditorSink);
		m_log_console.set_sink(&sink);

		// clang-format off
		m_autosave.repeat(true);
		m_autosave.set([&]() {
            save_project(false);
		}, 300000);
		// clang-format on

		m_settings_panel.load(core::ServiceLocator<core::Config>::ref().raw());
	}

	Editor::~Editor()
	{
		unload();
	}

	void Editor::load()
	{
		auto& win = core::ServiceLocator<core::Window>::ref();
		glfwSetWindowCloseCallback(win.handle(), [](GLFWwindow* window) {
			glfwSetWindowShouldClose(window, GLFW_FALSE);
		});

		m_editor_camera.set_viewport(win.get_widthf(), win.get_heightf());
		new_project();
	}

	void Editor::unload()
	{
		m_ldtk.terminate();
		m_bfxr.terminate();
	}

	void Editor::update()
	{
		for (const auto& task : m_tasks)
		{
			task();
		}

		m_tasks.clear();

		switch (m_state)
		{
			case EditorState::EDITOR:
				{
					m_autosave.update();
					m_project.only_update_rendering();

					if (m_viewport.m_viewport_focused && m_viewport.m_viewport_hovered)
					{
						if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
						{
							auto [mx, my]  = ImGui::GetMousePos();
							mx            -= m_viewport.m_viewport_bounds[0].x;
							my            -= m_viewport.m_viewport_bounds[0].y;

							const auto size = m_viewport.m_viewport_bounds[1] - m_viewport.m_viewport_bounds[0];
							my              = size.y - my;

							if (mx >= 0 && my >= 0 && mx < size.x && my < size.y)
							{
								auto& fb = m_viewport.m_framebuffer.fbo();

								const auto entity = fb.read_storagebuffer(static_cast<int>(mx), static_cast<int>(my));
								if (entity == -1)
								{
									m_selected.entity = entt::null;
									m_selected.scene  = nullptr;
								}
								else
								{
									m_selected.entity = static_cast<entt::entity>(static_cast<std::uint32_t>(entity));
									m_selected.scene  = m_project.current();
								}
							}
						}

						if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && m_viewport.m_editor_cam_enabled)
						{
							m_use_hand_cursor = true;

							m_imgui_mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
							m_editor_camera.translate(m_imgui_mouse_delta.x, m_imgui_mouse_delta.y);
							ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
						}
					}
				}
				break;

			case EditorState::GAME:
				{
					m_project.update();

					// We need to use galaxy event handling here since imgui is disabled.
					if (input::Input::key_down(input::Keys::KEY_PAUSE))
					{
						set_state_editor();
					}
				}
				break;

			case EditorState::GAME_IN_EDITOR:
				{
					m_project.update();

					if (m_viewport.m_viewport_focused && m_viewport.m_viewport_hovered)
					{
						set_input_game();
					}
					else
					{
						set_input_editor();
					}
				}
				break;
		}
	}

	void Editor::render()
	{
		switch (m_state)
		{
			case EditorState::EDITOR:
			case EditorState::GAME_IN_EDITOR:
				{
					if (m_project.current())
					{
						m_viewport.set_active();

						if (m_state == EditorState::EDITOR && m_viewport.m_editor_cam_enabled)
						{
							graphics::Renderer::ref().submit_camera(m_editor_camera);
						}
						else
						{
							graphics::Renderer::ref().submit_camera(m_project.current()->m_camera);
						}

						graphics::Renderer::ref().draw();

						auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();

						nui.new_frame();
						m_project.current()->update_ui();
						nui.render();
					}

					graphics::Renderer::ref().begin_default();
					draw_editor();
					graphics::Renderer::ref().end_default();
				}
				break;

			case EditorState::GAME:
				{
					// Screen-space post processing only available on full game.
					// TODO: Allow on any screen.
					m_project.render();
				}
				break;
		}
	}

	void Editor::exit()
	{
		core::ServiceLocator<core::Window>::ref().close();
	}

	void Editor::new_project()
	{
		m_project.clear();
		m_current_project_path = "";
		core::ServiceLocator<core::Window>::ref().append_title(" | Untitled");

		m_autosave.stop();
		m_autosave.start();
	}

	void Editor::load_project(const std::string& path)
	{
		std::ifstream ifs {path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate};

		if (ifs.good())
		{
			meta::vector<char> buffer;

			const auto size = ifs.tellg();
			buffer.resize(size);

			ifs.seekg(0, std::ifstream::beg);
			ifs.read(&buffer[0], size);
			ifs.close();

			m_current_project_path = std::filesystem::path(path);

			auto data = std::string(buffer.begin(), buffer.end());
			data      = math::decode_zlib(data);
			data      = math::decode_base64(data);

			auto json = json::read_raw(data);

			if (!json.empty())
			{
				core::ServiceLocator<core::Window>::ref().append_title(" | " + m_current_project_path.filename().stem().string());
				m_backup = json.at("app_data");

				m_tasks.emplace_back([&]() {
					// Sets from backup.
					set_state_editor();
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", path);
				ui::imgui_notify_error("Failed to open project.");
			}
		}
		else
		{
			ifs.close();

			GALAXY_LOG(GALAXY_ERROR, "Failed to open project file: {0}.", path);
			ui::imgui_notify_error("Failed to open project.");
		}
	}

	void Editor::save_project(bool save_as)
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		if (m_current_project_path.empty() || save_as)
		{
			auto sp_opt = fs.open_save_dialog("untitled.scproj", {"*.scproj"});
			if (!sp_opt.empty())
			{
				m_current_project_path = sp_opt;
			}
		}

		if (!m_current_project_path.empty())
		{
			std::ofstream ofs {m_current_project_path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};

			if (ofs.good())
			{
				auto& config = core::ServiceLocator<core::Config>::ref();
				config.raw(m_settings_panel.save());
				config.save();

				nlohmann::json out_json = "{\"settings\":{},\"app_data\":{}}"_json;

				out_json["app_data"] = m_project.serialize();
				out_json["settings"] = m_settings_panel.save();

				auto data = out_json.dump(4);

				data = math::encode_base64(data);
				data = math::encode_zlib(data);

				ofs.write(data.data(), data.size());

				core::ServiceLocator<core::Window>::ref().append_title("| " + m_current_project_path.filename().stem().string());
				ui::imgui_notify_info("Saved project.");
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save project.");
				ui::imgui_notify_error("Failed to save project.");
			}

			ofs.close();
		}
	}

	void Editor::export_project()
	{
		save_project();

		m_show_export = true;

		auto& tp = core::ServiceLocator<BS::thread_pool>::ref();
		tp.detach_task([&]() {
			const std::filesystem::path path = GALAXY_EDITOR_DATA_DIR + "export/";
			if (!std::filesystem::exists(path))
			{
				std::filesystem::create_directories(path);
			}

			std::ofstream app_data;
			std::ofstream app_config;

			auto data = m_project.serialize().dump(4);
			data      = math::encode_base64(data);
			data      = math::encode_zlib(data);

			app_data.open(path / GALAXY_APPDATA, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
			if (app_data.good())
			{
				app_data.write(data.data(), data.size());
			}

			app_data.close();

			auto config_data = m_settings_panel.save().dump(4);
			strutils::replace_first(config_data, "\"use_loose_assets\": true", "\"use_loose_assets\": false");

			app_config.open(path / "config.json", std::ofstream::out | std::ofstream::trunc);
			if (app_config.good())
			{
				app_config.write(config_data.data(), config_data.size());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to export project settings.");
			}

			app_config.close();

			const auto zip_path = path / GALAXY_ASSET_PACK;
			std::filesystem::remove(zip_path);

			struct zip_t* zip = zip_open(zip_path.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
			recursively_zip_assets(zip, GALAXY_ASSET_DIR);
			zip_close(zip);

			m_show_export = false;
		});
	}

	void Editor::draw_editor()
	{
		begin_dock();

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && m_viewport.m_editor_cam_enabled)
		{
			m_use_hand_cursor = false;
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}

		if (m_use_hand_cursor)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		draw_menubar();
		short_cuts();

		ui::imgui_popup("New##MenuBarPopup", m_show_new, [&]() {
			ui::imgui_confirm("Create a new project?\nUnsaved progress will be lost.", [&]() {
				new_project();
			});
		});

		ui::imgui_popup("Restart##MenuBarPopup", m_show_restart, [&]() {
			ui::imgui_confirm("Are you sure you want to restart?\nUnsaved progress will be lost.", [&]() {
				GALAXY_RESTART = true;
				exit();
			});
		});

		ui::imgui_popup("Exit##MenuBarPopup", m_show_exit, [&]() {
			ui::imgui_confirm("Are you sure you want to exit?\nUnsaved progress will be lost.", [&]() {
				exit();
			});
		});

		ui::imgui_popup("About##MenuBarPopup", m_show_about, [&]() {
			ImGui::Text("Galaxy Engine - Supercluster Editor\nLicensed under Apache 2.0.");
			ImGui::Spacing();
			ImGui::Text("Controls:\n[PAUSE] - Exit Game Mode.\n[LEFT SHIFT] - Enable Docking.");
		});

		m_log_console.render();
		m_lua_console.render();
		m_code_editor.render();
		m_asset_panel.render(m_code_editor);
		m_scene_panel.render(m_project, m_tasks, m_selected);
		m_entity_panel.render();
		m_viewport.render(m_state);
		m_settings_panel.render();

		render_exporter();
		end_dock();
	}

	void Editor::begin_dock()
	{
		ui::imgui_new_frame();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::Begin("Main Viewport",
			nullptr,
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("Main Viewport Dockspace"), {0, 0}, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void Editor::end_dock()
	{
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);

		ui::imgui_render();
	}

	void Editor::draw_menubar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				m_show_new = ImGui::MenuItem("New", "Ctrl+N");

				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					show_loadproject();
				}

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					save_project();
				}

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					save_project(true);
				}

				if (ImGui::MenuItem("Export", "Ctrl+E"))
				{
					export_project();
				}

				m_show_restart = ImGui::MenuItem("Restart");
				m_show_exit    = ImGui::MenuItem("Exit");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Panels"))
			{
				if (ImGui::MenuItem("Toggle Scene Panel", "Ctrl+Alt+S"))
				{
					m_scene_panel.m_show = !m_scene_panel.m_show;
				}

				if (ImGui::MenuItem("Toggle Entity Panel", "Ctrl+Alt+E"))
				{
					m_entity_panel.m_show = !m_entity_panel.m_show;
				}

				if (ImGui::MenuItem("Toggle Viewport", "Ctrl+Alt+V"))
				{
					m_viewport.m_show = !m_viewport.m_show;
				}

				if (ImGui::MenuItem("Toggle Asset Panel", "Ctrl+Alt+A"))
				{
					m_asset_panel.m_show = !m_asset_panel.m_show;
				}

				if (ImGui::MenuItem("Toggle Logs", "Ctrl+Alt+L"))
				{
					m_log_console.m_show = !m_log_console.m_show;
				}

				if (ImGui::MenuItem("Toggle Lua Terminal", "Ctrl+Alt+T"))
				{
					m_lua_console.m_show = !m_lua_console.m_show;
				}

				if (ImGui::MenuItem("Toggle Code Editor", "Ctrl+Alt+C"))
				{
					m_code_editor.m_show = !m_code_editor.m_show;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				static auto s_basedir = GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR;
				static auto s_ltdk    = s_basedir / "tools/ldtk/ldtk.exe";
				static auto s_bfxr    = s_basedir / "tools/bfxr/Bfxr.exe";

				if (ImGui::MenuItem("LDTK"))
				{
					m_ldtk.create(s_ltdk.string());
				}

				if (ImGui::MenuItem("BFXR"))
				{
					m_bfxr.create(s_bfxr.string());
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Settings"))
			{
				m_settings_panel.m_show = !m_settings_panel.m_show;
			}

			m_show_about = ImGui::MenuItem("About");

			draw_game_controls();

			const auto info = std::format("UPS: {0}, FPS: {1}", GALAXY_CUR_UPS, GALAXY_CUR_FPS);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(info.c_str()).x - m_padding.x);
			ImGui::TextUnformatted(info.c_str());
			ImGui::EndMenuBar();
		}
	}

	void Editor::draw_game_controls()
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);

		// We dont need to parse EditorState::GAME here.
		switch (m_state)
		{
			case EditorState::EDITOR:
				{
					if (ImGui::Button(ICON_MDI_PLAY, {24, 24}))
					{
						set_state_game_and_editor();
					}
				}
				break;

			case EditorState::GAME_IN_EDITOR:
				{
					if (ImGui::Button(ICON_MDI_STOP, {24, 24}))
					{
						set_state_editor();
					}
				}
				break;
		}

		// Now draw full game mode play button control.
		if (ImGui::Button(ICON_MDI_BUG_PLAY, {24, 24}))
		{
			if (!m_project.empty())
			{
				set_state_game();
			}
			else
			{
				ui::imgui_notify_warning("No active scene.");
			}
		}
	}

	void Editor::short_cuts()
	{
		m_show_new = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_N);

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_E))
		{
			export_project();
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_O))
		{
			show_loadproject();
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_L))
		{
			m_log_console.m_show = !m_log_console.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S))
		{
			save_project();
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_S))
		{
			save_project(true);
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_V))
		{
			m_viewport.m_show = !m_viewport.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_T))
		{
			m_lua_console.m_show = !m_lua_console.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_C))
		{
			m_code_editor.m_show = !m_code_editor.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_A))
		{
			m_asset_panel.m_show = !m_asset_panel.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_S))
		{
			m_scene_panel.m_show = !m_scene_panel.m_show;
		}

		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_E))
		{
			m_entity_panel.m_show = !m_entity_panel.m_show;
		}

		// static bool open = true;
		// ImGui::ShowDemoWindow(&open);
	}

	void Editor::show_loadproject()
	{
		const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.scproj"}, "../editor_data/projects");
		if (!path.empty())
		{
			load_project(path);
		}
		else
		{
			ui::imgui_notify_warning("No project file selected.");
		}
	}

	void Editor::render_exporter() const
	{
		if (m_show_export)
		{
			ui::imgui_center_next_window();
			if (ImGui::Begin("Exporting...",
					nullptr,
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs |
						ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove))
			{
				ImGuiContext& g      = *GImGui;
				ImGuiWindow*  window = ImGui::GetCurrentWindow();

				ImGuiStyle& style = g.Style;
				ImVec2      size  = ImGui::CalcItemSize(ImVec2(300, 20), ImGui::CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
				ImVec2      pos   = window->DC.CursorPos;
				ImRect      bb(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
				ImGui::ItemSize(size);
				if (!ImGui::ItemAdd(bb, 0))
					return;

				const float speed            = g.FontSize * 0.05f;
				const float phase            = ImFmod((float)g.Time * speed, 1.0f);
				const float width_normalized = 0.2f;
				float       t0               = phase * (1.0f + width_normalized) - width_normalized;
				float       t1               = t0 + width_normalized;

				ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
				bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
				ImGui::RenderRectFilledRangeH(window->DrawList, bb, ImGui::GetColorU32(ImGuiCol_PlotHistogram), t0, t1, style.FrameRounding);
			}

			ImGui::End();
		}
	}

	void Editor::recursively_zip_assets(zip_t* zip, const std::filesystem::path& path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			const auto entry_path = std::filesystem::path(entry);

			if (entry.is_directory())
			{
				recursively_zip_assets(zip, entry_path);
			}
			else
			{
				zip_entry_open(zip, entry_path.relative_path().string().c_str());
				zip_entry_fwrite(zip, entry_path.string().c_str());
				zip_entry_close(zip);
			}
		}
	}

	void Editor::set_state_game()
	{
		set_input_game();
		m_state = EditorState::GAME;

		m_tasks.push_back([&]() {
			if (m_state == EditorState::GAME_IN_EDITOR)
			{
				m_project.deserialize(m_backup);
			}
			else
			{
				m_backup = m_project.serialize();
			}
		});
	}

	void Editor::set_state_editor()
	{
		set_input_editor();
		m_state = EditorState::EDITOR;

		m_tasks.push_back([&]() {
			m_project.deserialize(m_backup);
		});
	}

	void Editor::set_state_game_and_editor()
	{
		m_backup = m_project.serialize();
		m_state  = EditorState::GAME_IN_EDITOR;
	}

	void Editor::set_input_game()
	{
		ImGui_ImplGlfw_ToggleInput(false);
		core::ServiceLocator<ui::NuklearUI>::ref().toggle_input(true);
		core::ServiceLocator<core::Window>::ref().set_dispatcher(&m_project.current()->m_dispatcher);
		core::ServiceLocator<core::Window>::ref().get_input<input::Cursor>().use_custom_else_pointer();
	}

	void Editor::set_input_editor()
	{
		ImGui_ImplGlfw_ToggleInput(true);
		core::ServiceLocator<ui::NuklearUI>::ref().toggle_input(false);
		core::ServiceLocator<core::Window>::ref().set_dispatcher(nullptr);
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
} // namespace sc
