///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <imgui_impl_glfw.h>
#include <imgui_addons/notify/imgui_notify.h>
#include <nlohmann/json.hpp>
#include <portable-file-dialogs.h>

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
	Editor::Editor(std::string_view name, state::Scene* scene) noexcept
		: Layer {name, scene}
	{
		m_code_editor.m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		m_code_editor.m_editor.SetPalette(TextEditor::GetDarkPalette());

		auto& sink = GALAXY_ADD_SINK(EditorSink);
		m_log_console.set_sink(&sink);

		m_framebuffer.create(1, 1);
		m_settings.load();

		m_resume_play.load("editor_data/icons/resume_play.png");
		m_resume_play.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_resume_play.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

		m_play.load("editor_data/icons/play.png");
		m_play.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_play.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

		m_stop.load("editor_data/icons/stop.png");
		m_stop.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_stop.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

		auto& config = core::ServiceLocator<core::Config>::ref();
		config.restore("autosave_interval_seconds", 300, "editor");

		// clang-format off
		m_autosave.repeat(true);
		m_autosave.set([&]() {
            save_project(false);
		}, config.get<int>("autosave_interval_seconds", "editor") * 1000);
		// clang-format on
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::on_push()
	{
		m_window->resize(1280, 720);
		m_window->maximize();

		m_autosave.start();
	}

	void Editor::on_pop()
	{
		m_autosave.stop();
	}

	void Editor::events()
	{
		if (!m_game_mode)
		{
			if (m_viewport_focused && m_viewport_hovered)
			{
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					m_mouse_picked = true;
				}

				if (m_project_scenes.has_current())
				{
					if (!m_paused)
					{
						ImGui_ImplGlfw_ToggleInput(true);
						m_project_scenes.current().events();
					}
				}
			}
			else
			{
				ImGui_ImplGlfw_ToggleInput(false);
			}
		}
		else
		{
			if (input::Input::key_down(input::Keys::ESCAPE))
			{
				ImGui_ImplGlfw_ToggleInput(false);
				m_game_mode = false;
				m_project_scenes.deserialize(m_backup);
			}
		}
	}

	void Editor::update()
	{
		if (!m_game_mode)
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
			m_autosave.update();
		}
		else
		{
			m_project_scenes.current().update();
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
			m_project_scenes.current().render();
		}
	}

	void Editor::new_project()
	{
		m_window->set_title("Untitled Project");

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

			auto decompressed   = std::string(buffer.begin(), buffer.end());
			auto decoded_zlib   = algorithm::decode_zlib(decompressed);
			auto decoded_base64 = algorithm::decode_base64(decoded_zlib);

			auto json = json::parse_from_mem(decoded_base64);

			if (json.has_value())
			{
				m_project_scenes.deserialize(json.value());
				m_window->set_title(fs_path.stem().string().c_str());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", fs_path.string());
				ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to open project."});
			}
		}
		else
		{
			ifs.close();

			GALAXY_LOG(GALAXY_ERROR, "Failed to open project file: {0}.", path);
			ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to open project."});
		}
	}

	void Editor::save_project(bool save_as)
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		if (m_current_project_path.empty() || save_as)
		{
			const auto sp_opt = fs.show_save_dialog("untitled.scproj", "*.scproj");
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
				auto data           = m_project_scenes.serialize().dump(4);
				auto encoded_base64 = algorithm::encode_base64(data);
				auto encoded_zlib   = algorithm::encode_zlib(encoded_base64);

				ofs.write(encoded_zlib.data(), encoded_zlib.size());
				ofs.close();

				m_window->set_title(std::filesystem::path(m_current_project_path).stem().string().c_str());
				ImGui_Notify::InsertNotification({ImGuiToastType_Info, 2000, "Saved project."});
			}
			else
			{
				ofs.close();

				GALAXY_LOG(GALAXY_ERROR, "Failed to save project to disk.");
				ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to save project."});
			}
		}
	}

	void Editor::restart()
	{
		GALAXY_RESTART = true;
		exit();
	}

	void Editor::exit()
	{
		m_window->close();
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

		if (!graphics::Renderer::get_data().empty())
		{
			m_render_data = graphics::Renderer::get_data();
		}

		auto& data = graphics::Renderer::get_data();
		data       = m_render_data;

		m_framebuffer.bind(true);
		if (m_project_scenes.has_current())
		{
			m_project_scenes.current().render();
		}

		graphics::Renderer::draw();
		graphics::Renderer::flush();

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

		static constexpr const auto size = ImVec2 {0.0f, 0.0f};

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, size);
		ImGui::Begin("Main Viewport", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("Main Viewport Dockspace"), size, dockspace_flags);
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
						auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.scproj", "editor_data/projects");
						if (file.has_value())
						{
							load_project(file.value());
						}
						else
						{
							ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to open project file."});
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

			if (ImGui::BeginMenu("View"))
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
				if (ImGui::MenuItem("Tiled", "Ctrl+Shift+T"))
				{
					// m_tiled_process.create("tools/tiled/tiled.exe");
				}

				ImGui::EndMenu();
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (m_icon_size.x * 2) - 8.0f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
			if (ImGui::Button(ICON_FA_COG))
			{
				m_show_settings = !m_show_settings;
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_icon_size.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
			ImGui::Text(ICON_FA_QUESTION_CIRCLE);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("supercluster v1.0\nLicensed under Apache 2.0.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
			if (!m_paused)
			{
				if (ui::imgui_imagebutton(m_stop, m_icon_size))
				{
					m_paused = true;
					m_project_scenes.deserialize(m_backup);
				}
			}
			else
			{
				if (ui::imgui_imagebutton(m_play, m_icon_size))
				{
					m_paused = false;
					m_backup = m_project_scenes.serialize();
				}
			}

			if (ui::imgui_imagebutton(m_resume_play, m_icon_size))
			{
				if (m_project_scenes.has_current())
				{
					ImGui_ImplGlfw_ToggleInput(true);

					m_game_mode = true;
					m_backup    = m_project_scenes.serialize();
				}
				else
				{
					ImGui_Notify::InsertNotification({ImGuiToastType_Warning, 2000, "No active scene."});
				}
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
			if (m_selected_entity.m_selected != entt::null)
			{
				ImGui::Text("Selected: %u", entt::to_integral(m_selected_entity.m_selected));
			}
			else
			{
				ImGui::Text("Selected: None");
			}

			ImGui::EndMenuBar();
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl, ImGuiKey_N))
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

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl, ImGuiKey_O))
		{
			m_update_stack.emplace_back([&]() {
				auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.scproj", "editor_data/projects");
			    if (file.has_value())
			    {
                    load_project(file.value());
			    }
            });
		}

		ui::imgui_confirm("RestartConfirm", [&](){
			restart();
		});

		ui::imgui_confirm("ExitConfirm", [&](){
			exit();
		});
		// clang-format on

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl, ImGuiKey_S))
		{
			save_project();
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Shift, ImGuiKey_S))
		{
			save_project(true);
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Shift, ImGuiKey_T))
		{
			// m_tiled_process.create("tools/tiled/tiled.exe");
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_R))
		{
			restart();
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_S))
		{
			m_show_scenes = !m_show_scenes;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_E))
		{
			m_show_entities = !m_show_entities;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_V))
		{
			m_show_viewport = !m_show_viewport;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_A))
		{
			m_show_assetpanel = !m_show_assetpanel;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_L))
		{
			m_show_logpanel = !m_show_logpanel;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_T))
		{
			m_show_luapanel = !m_show_luapanel;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_J))
		{
			m_show_jsonpanel = !m_show_jsonpanel;
		}

		if (ui::imgui_shortcut(ImGuiModFlags_Ctrl | ImGuiModFlags_Alt, ImGuiKey_C))
		{
			m_show_codeeditor = !m_show_codeeditor;
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
			code_editor();
		}

		if (m_show_viewport)
		{
			viewport();
		}

		if (m_show_assetpanel)
		{
			m_asset_panel.render(m_code_editor, m_update_stack);
		}

		if (m_show_scenes)
		{
			m_scene_panel.render(m_project_scenes, m_selected_entity);
		}

		if (m_show_entities)
		{
			m_entity_panel.render(m_selected_entity, m_update_stack);
		}

		//
		// UTILITY WINDOWS.
		//
		if (m_show_settings)
		{
			if (ImGui::Begin("Settings", &m_show_settings, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Save"))
					{
						m_settings.save();
						ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "Settings changed, a restart is needed."});
					}

					if (ImGui::MenuItem("Refresh"))
					{
						m_settings.load();
					}

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

						ImGui::EndMenu();
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

		ImGui::End();

		static constexpr const auto s_notification_size = ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, s_notification_size);
		ImGui_Notify::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		ui::imgui_prerender();
		ui::imgui_render();
	}

	void Editor::code_editor()
	{
		ImGui::Begin("CodeEditor", nullptr, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			code_editor_menu();
			ImGui::EndMenuBar();
		}

		ui::imgui_confirm("NewCodeEditorFile", [&]() {
			m_code_editor.m_editor.SetText("");
			m_code_editor.m_file = "";
		});

		const auto cpos = m_code_editor.m_editor.GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines | %s%s",
			cpos.mLine + 1,
			cpos.mColumn + 1,
			m_code_editor.m_editor.GetTotalLines(),
			m_code_editor.m_file.empty() ? "New File" : m_code_editor.m_file.string().c_str(),
			m_code_editor.m_editor.CanUndo() ? "*" : "");

		m_code_editor.m_editor.Render("TextEditor");
		if (ImGui::BeginDragDropTarget())
		{
			if (const auto payload = ImGui::AcceptDragDropPayload("AssetPanelItem"))
			{
				const char* path = static_cast<const char*>(payload->Data);

				const auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().open(path);
				if (file.has_value())
				{
					m_code_editor.m_editor.SetText(file.value());
					m_code_editor.m_file = std::filesystem::path(path);
				}
				else
				{
					ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to open script."});
				}
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("CodeEditorContextMenu");
		}

		if (ImGui::BeginPopupContextWindow("CodeEditorContextMenu"))
		{
			code_editor_menu();
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void Editor::code_editor_menu()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				if (!m_code_editor.m_editor.GetText().empty())
				{
					ui::imgui_open_confirm("NewCodeEditorFile");
				}
				else
				{
					m_code_editor.m_file = "";
				}
			}

			if (ImGui::MenuItem("Open"))
			{
				auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
				const auto file = fs.show_open_dialog("*.lua", "scripts");

				if (file.has_value())
				{
					m_code_editor.m_editor.SetText(fs.open(file.value()).value());
					m_code_editor.m_file = file.value();
				}
				else
				{
					ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to open script."});
				}
			}

			if (ImGui::MenuItem("Save"))
			{
				if (m_code_editor.m_file.empty())
				{
					auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
					const auto file = fs.show_save_dialog("untitled.lua", "*.lua");

					if (file.has_value())
					{
						m_code_editor.m_file = file.value();
					}

					if (!fs.save(m_code_editor.m_editor.GetText(), m_code_editor.m_file.string()))
					{
						ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to save script file."});
					}
				}
			}

			if (ImGui::MenuItem("Save as..."))
			{
				if (!core::ServiceLocator<fs::VirtualFileSystem>::ref().save_with_dialog(m_code_editor.m_editor.GetText(), "untitled.lua"))
				{
					ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to save script file."});
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			auto ro = m_code_editor.m_editor.IsReadOnly();
			if (ImGui::MenuItem("Read only", nullptr, &ro))
			{
				m_code_editor.m_editor.SetReadOnly(ro);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "Ctrl+Z | Alt+Backspace", nullptr, !ro && m_code_editor.m_editor.CanUndo()))
			{
				m_code_editor.m_editor.Undo();
			}

			if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, !ro && m_code_editor.m_editor.CanRedo()))
			{
				m_code_editor.m_editor.Redo();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, m_code_editor.m_editor.HasSelection()))
			{
				m_code_editor.m_editor.Copy();
			}

			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, !ro && m_code_editor.m_editor.HasSelection()))
			{
				m_code_editor.m_editor.Cut();
			}

			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_code_editor.m_editor.HasSelection()))
			{
				m_code_editor.m_editor.Delete();
			}

			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
			{
				m_code_editor.m_editor.Paste();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", "Ctrl+A", nullptr))
			{
				m_code_editor.m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_code_editor.m_editor.GetTotalLines(), 0));
			}

			ImGui::EndMenu();
		}
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

				if (m_project_scenes.has_current())
				{
					m_project_scenes.current().get_camera().on_window_resized({.m_width = (int)m_viewport_size.x, .m_height = (int)m_viewport_size.y});
				}
			}

			ImGui::Image(reinterpret_cast<void*>(m_framebuffer.get_texture()), m_viewport_size, {0, 1}, {1, 0});

			if (m_mouse_picked)
			{
				// auto& camera = m_project_scenes.current().get_camera();

				// const auto x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - camera.get_x();
				// const auto y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - camera.get_y();

				m_mouse_picked = false;
			}
		}

		ImGui::PopStyleVar(1);
		ImGui::End();
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