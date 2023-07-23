///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <BS_thread_pool.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_addons/code_editor/ImGuiController.h>
#include <imgui_addons/imgui_notify.h>
#include <nlohmann/json.hpp>

#include <galaxy/algorithm/Base64.hpp>
#include <galaxy/algorithm/ZLib.hpp>
#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/input/Input.hpp>
#include <galaxy/scripting/JSON.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor(const std::string& name)
		: Scene(name)
		, m_editor_camera {true}
	{
		ste::CodeEditor::Setup(core::ServiceLocator<fs::VirtualFileSystem>::ref().root());

#ifdef _DEBUG
		auto& sink = GALAXY_ADD_SINK(EditorSink);
		m_log_console.set_sink(&sink);
#endif

		m_framebuffer.create(1, 1);
		m_settings.load(core::ServiceLocator<core::Config>::ref().raw());

		auto& config = core::ServiceLocator<core::Config>::ref();
		config.restore("autosave_interval_seconds", 300, "editor");

		// clang-format off
		m_autosave.repeat(true);
		m_autosave.set([&]() {
            save_project(false);
		}, config.get<int>("autosave_interval_seconds", "editor") * 1000);
		// clang-format on

		auto& fb           = m_framebuffer.get_framebuffer();
		m_mousepick_buffer = fb.add_storage_attachment();
		fb.create();

		m_camera_btn.load("../editor_data/icons/camera.png");
		m_camera_btn.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_camera_btn.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

		m_editor_cam_btn.load("../editor_data/icons/video.png");
		m_editor_cam_btn.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_editor_cam_btn.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
	}

	Editor::~Editor()
	{
	}

	void Editor::load()
	{
		auto& window = core::ServiceLocator<core::Window>::ref();
		window.resize(1280, 720);
		window.maximize();

		m_editor_camera.set_viewport(window.get_widthf(), window.get_heightf());

		m_autosave.start();
	}

	void Editor::unload()
	{
		m_autosave.stop();
	}

	void Editor::update()
	{
		for (const auto& update : m_update_stack)
		{
			update();
		}

		m_update_stack.clear();

		if (!m_game_mode)
		{
			if (m_stopped)
			{
				if (m_viewport_focused && m_viewport_hovered)
				{
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
					{
						auto [mx, my] = ImGui::GetMousePos();
						mx -= m_viewport_bounds[0].x;
						my -= m_viewport_bounds[0].y;

						const auto size = m_viewport_bounds[1] - m_viewport_bounds[0];
						my              = size.y - my;

						if (mx >= 0 && my >= 0 && mx < size.x && my < size.y)
						{
							auto& fb = m_framebuffer.get_framebuffer();

							const auto entity = fb.read_storagebuffer(m_mousepick_buffer, static_cast<int>(mx), static_cast<int>(my));
							if (entity == -1)
							{
								m_selected_entity.m_selected = entt::null;
								m_selected_entity.m_world    = nullptr;
							}
							else
							{
								m_selected_entity.m_selected = static_cast<entt::entity>(static_cast<std::uint32_t>(entity));
								m_selected_entity.m_world    = &m_project_sm.current().m_world;
							}
						}
					}

					if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && m_editor_cam_enabled)
					{
						m_use_hand = true;

						m_imgui_mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
						m_editor_camera.translate(m_imgui_mouse_delta.x, m_imgui_mouse_delta.y);
						ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
					}
				}

				if (m_project_sm.has_current())
				{
					m_project_sm.current().m_world.update_rendersystem();
					graphics::Renderer::buffer_camera(m_project_sm.current().m_camera);
				}
			}
			else
			{
				m_project_sm.current().update();

				if (input::Input::key_down(input::Keys::LEFT_ALT))
				{
					ImGui_ImplGlfw_ToggleInput(false);
				}
				else
				{
					ImGui_ImplGlfw_ToggleInput(true);
				}
			}

			m_autosave.update();
		}
		else
		{
			if (input::Input::key_down(input::Keys::LEFT_SHIFT) && input::Input::key_down(input::Keys::TAB))
			{
				ImGui_ImplGlfw_ToggleInput(false);
				ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

				m_game_mode = false;

				m_update_stack.push_back([&]() {
					m_project_sm.deserialize(m_backup);
				});
			}
			else
			{
				m_project_sm.current().update();
			}
		}
	}

	void Editor::render()
	{
		if (!m_game_mode)
		{
			draw_editor();
		}
		else
		{
			m_project_sm.current().render();
		}
	}

	void Editor::new_project()
	{
		core::ServiceLocator<core::Window>::ref().set_title("Untitled Project");
		m_project_sm.clear();
	}

	void Editor::load_project(std::string_view path)
	{
		std::ifstream ifs {std::string(path), std::ifstream::in | std::ifstream::binary | std::ifstream::ate};

		if (ifs.good())
		{
			meta::vector<char> buffer;

			const auto size = ifs.tellg();
			buffer.resize(size);

			ifs.seekg(0, std::ifstream::beg);
			ifs.read(&buffer[0], size);
			ifs.close();

			const auto fs_path     = std::filesystem::path(path);
			m_current_project_path = fs_path.string();

			auto data = std::string(buffer.begin(), buffer.end());

#ifndef _DEBUG
			data = algorithm::decode_zlib(data);
			data = algorithm::decode_base64(data);
#endif

			auto json = json::parse_from_mem(data);

			if (json.has_value())
			{
				const auto& scenes = json.value().at("app_data");

				m_project_sm.deserialize(scenes);
				core::ServiceLocator<core::Window>::ref().set_title(fs_path.stem().string().c_str());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", fs_path.string());
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
				m_current_project_path = std::move(sp_opt);
			}
		}

		if (!m_current_project_path.empty())
		{
			std::ofstream ofs {m_current_project_path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};

			if (ofs.good())
			{
				auto& config = core::ServiceLocator<core::Config>::ref();
				config.raw(m_settings.save());
				config.save();

				nlohmann::json out_json = "{\"settings\":{},\"app_data\":{}}"_json;

				out_json["app_data"] = m_project_sm.serialize();
				out_json["settings"] = m_settings.save();

				auto data = out_json.dump(4);

#ifndef _DEBUG
				data = algorithm::encode_base64(data);
				data = algorithm::encode_zlib(data);
#endif

				ofs.write(data.data(), data.size());

				core::ServiceLocator<core::Window>::ref().set_title(std::filesystem::path(m_current_project_path).stem().string().c_str());
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

		m_show_exportprogress = true;

		auto& tp = core::ServiceLocator<BS::thread_pool>::ref();
		tp.push_task([&]() {
			auto& config    = core::ServiceLocator<core::Config>::ref();
			const auto path = std::filesystem::path("export");
			if (!std::filesystem::exists(path))
			{
				std::filesystem::create_directories(path);
			}

			std::ofstream app_data;
			std::ofstream app_config;

			auto data = m_project_sm.serialize().dump(4);
			data      = algorithm::encode_base64(data);
			data      = algorithm::encode_zlib(data);

			app_data.open(path / config.get<std::string>("app_data"), std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
			if (app_data.good())
			{
				app_data.write(data.data(), data.size());
			}

			app_data.close();

			auto config_data = m_settings.save().dump(4);

			app_config.open(path / "config.json", std::ofstream::out | std::ofstream::trunc);
			if (app_config.good())
			{
				app_config.write(config_data.data(), config_data.size());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to export project.");
			}

			app_config.close();

			const auto zip_path = path / "assets.zip";
			std::filesystem::remove(zip_path);

			struct zip_t* zip = zip_open(zip_path.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
			recursively_zip_assets(zip, config.get<std::string>("asset_dir"));
			zip_close(zip);

			m_show_exportprogress = false;
		});
	}

	void Editor::restart()
	{
		GALAXY_RESTART = true;
		exit();
	}

	void Editor::exit()
	{
		m_tiled_process.terminate();
		m_bfxr_process.terminate();
		core::ServiceLocator<core::Window>::ref().close();
	}

	void Editor::draw_editor()
	{
#ifdef _DEBUG
		static bool s_show_demo = false;
#endif
		static GLint s_cur_fbo = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_cur_fbo);

		static GLint s_viewport[4] = {0, 0, 0, 0};
		glGetIntegerv(GL_VIEWPORT, s_viewport);

		m_framebuffer.bind(true);
		m_framebuffer.get_framebuffer().clear_storagebuffer(m_mousepick_buffer, -1);

		if (m_project_sm.has_current())
		{
			if (m_stopped && m_editor_cam_enabled)
			{
				graphics::Renderer::buffer_camera(m_editor_camera);
			}

			m_project_sm.current().render();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
		glViewport(s_viewport[0], s_viewport[1], s_viewport[2], s_viewport[3]);

		ui::imgui_new_frame();

		// Cursor handling here due to issues with fixed timestep loop.

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && m_editor_cam_enabled)
		{
			m_use_hand = false;
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}

		if (m_use_hand)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
															   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
															   ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_no_padding);
		ImGui::Begin("Main Viewport", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("Main Viewport Dockspace"), m_no_padding, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					ui::imgui_open_confirm("NewConfirmPopup");
				}

				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					m_update_stack.emplace_back([&]() {
						const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.scproj"}, "../editor_data/projects");
						if (!path.empty())
						{
							load_project(path);
						}
						else
						{
							ui::imgui_notify_error("Failed to open project file.");
						}
					});
				}

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					save_project();
				}

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					save_project(true);
				}

				if (ImGui::MenuItem("Export"))
				{
					export_project();
				}

				if (ImGui::MenuItem("Restart", "Ctrl+Alt+R"))
				{
					ui::imgui_open_confirm("RestartConfirm");
				}

				if (ImGui::MenuItem("Exit"))
				{
					ui::imgui_open_confirm("ExitConfirm");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Panels"))
			{
				if (ImGui::MenuItem("Toggle Scene Panel", "Ctrl+Alt+S"))
				{
					m_show_scenes = !m_show_scenes;
				}

				if (ImGui::MenuItem("Toggle Entity Panel", "Ctrl+Alt+E"))
				{
					m_show_entities = !m_show_entities;
				}

				if (ImGui::MenuItem("Toggle Viewport", "Ctrl+Alt+V"))
				{
					m_show_viewport = !m_show_viewport;
				}

				if (ImGui::MenuItem("Toggle Asset Panel", "Ctrl+Alt+A"))
				{
					m_show_assetpanel = !m_show_assetpanel;
				}

				if (ImGui::MenuItem("Toggle Logs", "Ctrl+Alt+L"))
				{
					m_show_logpanel = !m_show_logpanel;
				}

				if (ImGui::MenuItem("Toggle Lua Terminal", "Ctrl+Alt+T"))
				{
					m_show_luapanel = !m_show_luapanel;
				}

				if (ImGui::MenuItem("Toggle JSON Editor", "Ctrl+Alt+J"))
				{
					m_show_jsonpanel = !m_show_jsonpanel;
				}

				if (ImGui::MenuItem("Toggle Code Editor", "Ctrl+Alt+C"))
				{
					m_show_codeeditor = !m_show_codeeditor;
				}

				if (ImGui::MenuItem("Toggle Noise Editor", "Ctrl+Alt+N"))
				{
					m_show_noisegraph = !m_show_noisegraph;
				}

#ifdef _DEBUG
				if (ImGui::MenuItem("ImGui Demo Window"))
				{
					GALAXY_LOG(GALAXY_INFO, "SHOWING DEBUG WINDOW.");
					s_show_demo = !s_show_demo;
				}
#endif

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Tiled"))
				{
					m_tiled_process.create(core::ServiceLocator<fs::VirtualFileSystem>::ref().root() + "../editor_data/tools/tiled/tiled.exe");
				}

				if (ImGui::MenuItem("Bfxr"))
				{
					m_bfxr_process.create(core::ServiceLocator<fs::VirtualFileSystem>::ref().root() + "../editor_data/tools/bfxr/Bfxr.exe");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Theme"))
			{
				auto& config = core::ServiceLocator<core::Config>::ref();

				if (ImGui::MenuItem("Dark"))
				{
					ImGui::StyleColorsDark();

					config.set<std::string>("theme", "DARK", "editor");
					config.save();
				}

				if (ImGui::MenuItem("Dracula"))
				{
					ui::imgui_theme_dracula();

					config.set<std::string>("theme", "DRACULA", "editor");
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

				if (ImGui::MenuItem("Enhanced Dark"))
				{
					ui::imgui_theme_enhanced_dark();

					config.set<std::string>("theme", "ENHANCED_DARK", "editor");
					config.save();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Settings"))
			{
				m_show_settings = !m_show_settings;
			}

			if (ImGui::MenuItem("About"))
			{
				m_show_about    = true;
				m_about_control = true;
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
			if (m_stopped)
			{
				if (ImGui::Button(ICON_MDI_PLAY, m_icon_size))
				{
					if (m_project_sm.has_current())
					{
						ImGui_ImplGlfw_ToggleInput(true);

						m_backup  = m_project_sm.serialize();
						m_stopped = false;
					}
					else
					{
						ui::imgui_notify_warning("No active scene.");
					}
				}
			}
			else
			{
				if (ImGui::Button(ICON_MDI_STOP, m_icon_size))
				{
					ImGui_ImplGlfw_ToggleInput(false);

					m_stopped = true;
					m_update_stack.push_back([&]() {
						m_project_sm.deserialize(m_backup);
					});
				}
			}

			if (ImGui::Button(ICON_MDI_BUG_PLAY, m_icon_size))
			{
				if (m_project_sm.has_current())
				{
					ImGui_ImplGlfw_ToggleInput(true);

					m_game_mode = true;
					m_backup    = m_project_sm.serialize();

					core::ServiceLocator<core::Window>::ref().get_input<input::Cursor>().use_custom_else_pointer();
				}
				else
				{
					ui::imgui_notify_warning("No active scene.");
				}
			}

			const auto info = std::format("UPS: {0}, FPS: {1}", GALAXY_CUR_UPS, GALAXY_CUR_FPS);

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(info.c_str()).x - m_padding.x);
			ImGui::TextUnformatted(info.c_str());

			ImGui::EndMenuBar();
		}

		if (m_show_about)
		{
			m_show_about = false;

			ImGui::OpenPopup("About##MenuBarAboutPopup");
		}

		if (ImGui::BeginPopupModal("About##MenuBarAboutPopup", &m_about_control))
		{
			ImGui::TextWrapped("Galaxy Game Engine\nSupercluster Editor\nLicensed under Apache 2.0.");
			ImGui::Spacing();
			ImGui::TextWrapped("Controls:\nTo exit game mode, press SHIFT+TAB.\nWhen viewport is running, hold LEFT ALT to restore ImGui input.\nHold Left "
							   "SHIFT to enable docking.");
			ImGui::EndPopup();
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl, ImGuiKey_N))
		{
			ui::imgui_open_confirm("NewConfirmPopup");
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
		// clang-format on

		if (ui::imgui_shortcut(ImGuiMod_Ctrl, ImGuiKey_O))
		{
			m_update_stack.emplace_back([&]() {
				const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.scproj"}, "../editor_data/projects");
				if (!path.empty())
				{
					load_project(path);
				}
			});
		}

		ui::imgui_confirm("RestartConfirm", [&]() {
			restart();
		});

		ui::imgui_confirm("ExitConfirm", [&]() {
			exit();
		});

		if (ui::imgui_shortcut(ImGuiMod_Ctrl, ImGuiKey_S))
		{
			save_project();
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Shift, ImGuiKey_S))
		{
			save_project(true);
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_R))
		{
			restart();
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_S))
		{
			m_show_scenes = !m_show_scenes;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_E))
		{
			m_show_entities = !m_show_entities;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_V))
		{
			m_show_viewport = !m_show_viewport;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_A))
		{
			m_show_assetpanel = !m_show_assetpanel;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_L))
		{
			m_show_logpanel = !m_show_logpanel;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_T))
		{
			m_show_luapanel = !m_show_luapanel;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_J))
		{
			m_show_jsonpanel = !m_show_jsonpanel;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_C))
		{
			m_show_codeeditor = !m_show_codeeditor;
		}

		if (ui::imgui_shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt, ImGuiKey_N))
		{
			m_show_noisegraph = !m_show_noisegraph;
		}

		if (m_show_noisegraph)
		{
			m_noise_graph.render(&m_show_noisegraph, m_update_stack);
		}

		if (m_show_luapanel)
		{
			m_lua_console.render();
		}

		if (m_show_logpanel)
		{
			m_log_console.render();
		}

		if (m_show_jsonpanel)
		{
			m_json_panel.render();
		}

		if (m_show_codeeditor)
		{
			ste::CodeEditor::Tick();
		}

		if (m_show_viewport)
		{
			viewport();
		}

		if (m_show_assetpanel)
		{
			m_asset_panel.render(m_update_stack);
		}

		if (m_show_scenes)
		{
			m_scene_panel.render(m_project_sm, m_selected_entity, m_update_stack);
		}

		if (m_show_entities)
		{
			m_entity_panel.render(m_selected_entity, m_update_stack);
		}

		if (m_show_settings)
		{
			if (ImGui::Begin("Settings", &m_show_settings, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Save"))
					{
						auto& config = core::ServiceLocator<core::Config>::ref();
						config.raw(m_settings.save());
						config.save();

						ui::imgui_notify_success("Settings changed, a restart is needed.");
					}

					if (ImGui::MenuItem("Refresh"))
					{
						m_settings.load(core::ServiceLocator<core::Config>::ref().raw());
					}

					ImGui::EndMenuBar();
				}

				m_settings.render();
			}

			ImGui::End();
		}

#ifdef _DEBUG
		if (s_show_demo)
		{
			ImGui::ShowDemoWindow(&s_show_demo);
		}
#endif

		if (m_show_exportprogress)
		{
			ui::imgui_center_next_window();
			if (ImGui::Begin("Exporting...",
					nullptr,
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs |
						ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove))
			{
				ImGuiContext& g     = *GImGui;
				ImGuiWindow* window = ImGui::GetCurrentWindow();

				ImGuiStyle& style = g.Style;
				ImVec2 size       = ImGui::CalcItemSize(ImVec2(300, 20), ImGui::CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
				ImVec2 pos        = window->DC.CursorPos;
				ImRect bb(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
				ImGui::ItemSize(size);
				if (!ImGui::ItemAdd(bb, 0))
					return;

				const float speed            = g.FontSize * 0.05f;
				const float phase            = ImFmod((float)g.Time * speed, 1.0f);
				const float width_normalized = 0.2f;
				float t0                     = phase * (1.0f + width_normalized) - width_normalized;
				float t1                     = t0 + width_normalized;

				ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
				bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
				ImGui::RenderRectFilledRangeH(window->DrawList, bb, ImGui::GetColorU32(ImGuiCol_PlotHistogram), t0, t1, style.FrameRounding);
			}

			ImGui::End();
		}

		ImGui::End();

		static constexpr const auto s_notification_size = ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, s_notification_size);
		ImGui_Notify::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		ui::imgui_render();
	}

	void Editor::viewport()
	{
		constexpr const ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_no_padding);
		if (ImGui::Begin(ICON_MDI_WINDOW_MAXIMIZE " Viewport", nullptr, flags))
		{
			const auto viewport_min = ImGui::GetWindowContentRegionMin();
			const auto viewport_max = ImGui::GetWindowContentRegionMax();
			const auto offset       = ImGui::GetWindowPos();

			m_viewport_bounds[0] = {viewport_min.x + offset.x, viewport_min.y + offset.y};
			m_viewport_bounds[1] = {viewport_max.x + offset.x, viewport_max.y + offset.y};

			m_viewport_focused = ImGui::IsWindowFocused();
			m_viewport_hovered = ImGui::IsWindowHovered();

			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.resize(static_cast<int>(m_viewport_size.x), static_cast<int>(m_viewport_size.y));
			}

			if (m_stopped)
			{
				ImGui::BeginGroup();
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_padding);

					ImGui::SetCursorPos(ImGui::GetCursorPos() + m_padding);
					if (ui::imgui_imagebutton(m_camera_btn, m_icon_size_large))
					{
						m_editor_cam_enabled = false;
					}

					ImGui::SameLine();

					if (ui::imgui_imagebutton(m_editor_cam_btn, m_icon_size_large))
					{
						m_editor_cam_enabled = true;
					}

					ImGui::PopStyleVar(2);
				}
				ImGui::EndGroup();
			}

			ui::imgui_image(m_framebuffer, m_viewport_size);
		}

		ImGui::PopStyleVar(1);
		ImGui::End();
	}

	void Editor::recursively_zip_assets(struct zip_t* zip, const std::filesystem::path& path)
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
} // namespace sc