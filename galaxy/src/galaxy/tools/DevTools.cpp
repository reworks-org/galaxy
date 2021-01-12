///
/// DevTools.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/addons/ToggleButton.h>

#include <portable-file-dialogs.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/StateMachine.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/scripting/JSONUtils.hpp"
#include "galaxy/graphics/sprite/BatchedSprite.hpp"
#include "galaxy/tools/ToolTheme.hpp"
#include "galaxy/audio/Sound.hpp"
#include "galaxy/audio/Music.hpp"
#include "galaxy/graphics/anim/AnimatedBatchSprite.hpp"
#include "galaxy/physics/body/BodyWrapper.hpp"
#include "galaxy/physics/body/KineticBody.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"
#include "galaxy/platform/Platform.hpp"

#include "DevTools.hpp"

namespace galaxy
{
	namespace tools
	{
		DevTools::DevTools()
		    : m_world {nullptr}, m_window {nullptr}, m_draw_main {true}, m_draw_demo {false}, m_draw_state_editor {false}, m_draw_json_editor {false}, m_draw_script_editor {false}, m_draw_atlas_editor {false}, m_draw_entity_editor {false}, m_draw_lua_console {false}, m_draw_gui_builder_ui {false}, m_atlas_state {-1}, m_show_entity_create {false}, m_entity_debug_name {"..."}, m_active_entity {0}, m_edn_buffer {""}
		{
		}

		DevTools::~DevTools()
		{
			m_world  = nullptr;
			m_window = nullptr;

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void DevTools::create()
		{
			m_window = SL_HANDLE.window();
			m_world  = SL_HANDLE.world();

			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigDockingAlwaysTabBar = true;
			io.ConfigDockingWithShift = true;
			// clang-format on

			tools::theme::visual_dark();
			ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);

			const constexpr char* gl_version = "#version 450 core";
			ImGui_ImplOpenGL3_Init(gl_version);

			m_editor.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::Lua());
		}

		void DevTools::prepare()
		{
			if (!m_sprites_to_create.empty())
			{
				for (auto& [sprite, path] : m_sprites_to_create)
				{
					sprite->load(path);
					sprite->create<graphics::BufferDynamic>();
				}

				m_sprites_to_create.clear();
			}

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
			ImGui::Begin("Dev Tools", &m_draw_main, window_flags);
			ImGui::PopStyleVar(3);

			ImGui::DockSpace(ImGui::GetID("DevTools_Dockspace_01"), {0.0f, 0.0f}, dockspace_flags);

			ImGui::SameLine();

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("[INFO]");
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted("[SHIFT] for docking.\n[GRAVE] to toggle.");
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}

				if (ImGui::BeginMenu("Menu"))
				{
					if (ImGui::MenuItem("Show ImGui::Demo"))
					{
						m_draw_demo = !m_draw_demo;
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
							tools::theme::enhanced_light();
						}

						if (ImGui::MenuItem("Enhanced Dark"))
						{
							tools::theme::enhanced_dark();
						}

						if (ImGui::MenuItem("Material Dark"))
						{
							tools::theme::material_dark();
						}

						if (ImGui::MenuItem("Visual Dark"))
						{
							tools::theme::visual_dark();
						}

						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Select Tiled exe"))
					{
						auto path    = fs::open_file_dialog("*.exe", "C:/");
						m_tiled_path = std::filesystem::path(path).string();
					}

					if (ImGui::MenuItem("Restart"))
					{
						SL_HANDLE.m_restart = true;
						m_window->close();
					}

					if (ImGui::MenuItem("Exit"))
					{
						m_window->close();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Gamestate Manager"))
				{
					m_draw_state_editor = !m_draw_state_editor;
				}

				if (ImGui::MenuItem("JSON Editor"))
				{
					m_draw_json_editor = !m_draw_json_editor;
				}

				if (ImGui::MenuItem("Lua Editor"))
				{
					m_draw_script_editor = !m_draw_script_editor;
				}

				if (ImGui::MenuItem("Texture Atlas Editor"))
				{
					m_draw_atlas_editor = !m_draw_atlas_editor;
				}

				if (ImGui::MenuItem("Entity Manager"))
				{
					m_draw_entity_editor = !m_draw_entity_editor;
				}

				if (ImGui::MenuItem("GUI Builder"))
				{
					m_draw_gui_builder_ui = !m_draw_gui_builder_ui;
				}

				if (ImGui::MenuItem("Lua Console"))
				{
					m_draw_lua_console = !m_draw_lua_console;
				}

				if (ImGui::MenuItem("Launch Tiled"))
				{
					if (!m_tiled_path.empty())
					{
						pfd::message("INFO", "Opening Tiled.\nApplication will be locked until closed.\nPress OK to continue.", pfd::choice::ok, pfd::icon::info);
						platform::run_process(m_tiled_path);
					}
					else
					{
						pfd::message("WARNING", "No tiled application specified.", pfd::choice::ok, pfd::icon::warning);
					}
				}

				ImGui::EndMenuBar();
			}

			if (m_draw_demo)
			{
				ImGui::ShowDemoWindow(&m_draw_demo);
			}

			if (m_draw_state_editor)
			{
				state_manager_ui();
			}

			if (m_draw_json_editor)
			{
				json_ui();
			}

			if (m_draw_script_editor)
			{
				script_editor_ui();
			}

			if (m_draw_atlas_editor)
			{
				atlas_editor_ui();
			}

			if (m_draw_entity_editor)
			{
				entity_ui();
			}

			if (m_draw_gui_builder_ui)
			{
				gui_builder_ui();
			}

			if (m_draw_lua_console)
			{
				m_console.draw(&m_draw_lua_console);
			}

			ImGui::End();
			end();
		}

		void DevTools::draw()
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void DevTools::start()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void DevTools::end()
		{
			ImGui::Render();
		}

		void DevTools::state_manager_ui()
		{
			auto* gs = SL_HANDLE.gamestate();

			ImGui::Begin("Gamestate Manager", &m_draw_state_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

			static std::string s_selected_state = gs->top<core::State>()->get_name();
			if (ImGui::BeginCombo("Select State", s_selected_state.c_str()))
			{
				for (const auto& state : gs->get_state_keys())
				{
					const bool selected = (s_selected_state == state);
					if (ImGui::Selectable(state.c_str(), selected))
					{
						s_selected_state = state;
					}

					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Pop##1"))
			{
				gs->pop();
			}

			ImGui::SameLine();

			if (ImGui::Button("Push##1"))
			{
				gs->push(s_selected_state);
			}

			ImGui::Spacing();
			auto* layers = gs->top<core::State>()->get_layers();

			static std::string s_selected_layer = "...";
			if (ImGui::BeginCombo("Select Layer", s_selected_layer.c_str()))
			{
				for (const auto& layer : layers->get_layer_keys())
				{
					const bool selected = (s_selected_layer == layer);
					if (ImGui::Selectable(layer.c_str(), selected))
					{
						s_selected_layer = layer;
					}

					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Pop##2"))
			{
				layers->pop();
			}

			ImGui::SameLine();

			if (ImGui::Button("Push##2"))
			{
				layers->push(s_selected_layer);
			}

			ImGui::End();
		}

		void DevTools::json_ui()
		{
			ImGui::Begin("JSON Editor", &m_draw_json_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::Text("Visual JSON editor.");
			ImGui::Spacing();

			if (ImGui::Button("New"))
			{
				ImGui::OpenPopup("create_new", ImGuiPopupFlags_NoOpenOverExistingPopup);
			}

			m_json_editor.create_new();
			ImGui::SameLine();

			if (ImGui::Button("Open"))
			{
				auto fp = fs::open_file_dialog("*.json");
				m_json_editor.load_file(fp);
			}

			ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				if (m_json_editor.is_loaded())
				{
					auto fp = fs::save_file_dialog();
					m_json_editor.save(fp);
				}
			}

			ImGui::Separator();
			ImGui::Spacing();

			m_json_editor.parse_and_display();

			ImGui::End();
		}

		void DevTools::script_editor_ui()
		{
			ImGui::Begin("Lua Editor", &m_draw_script_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
			ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						auto fp = std::filesystem::path {fs::open_file_dialog("*.lua")};
						std::ifstream text(fp.string(), std::ifstream::in);
						if (text.good())
						{
							std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
							m_editor.SetText(str);
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to read file: {0}.", fp.string());
						}

						text.close();
					}

					if (ImGui::MenuItem("Save"))
					{
						auto fp   = std::filesystem::path {fs::save_file_dialog()};
						auto text = m_editor.GetText();

						std::ofstream out(fp.string(), std::ios::out | std::ios::trunc);
						out << text;
						out.close();
					}

					if (ImGui::MenuItem("Close"))
					{
						m_draw_script_editor = false;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = m_editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					{
						m_editor.SetReadOnly(ro);
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "", nullptr, !ro && m_editor.CanUndo()))
					{
						m_editor.Undo();
					}

					if (ImGui::MenuItem("Redo", "", nullptr, !ro && m_editor.CanRedo()))
					{
						m_editor.Redo();
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", "", nullptr, m_editor.HasSelection()))
					{
						m_editor.Copy();
					}

					if (ImGui::MenuItem("Cut", "", nullptr, !ro && m_editor.HasSelection()))
					{
						m_editor.Cut();
					}

					if (ImGui::MenuItem("Delete", "", nullptr, !ro && m_editor.HasSelection()))
					{
						m_editor.Delete();
					}

					if (ImGui::MenuItem("Paste", "", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					{
						m_editor.Paste();
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Select all", nullptr, nullptr))
					{
						m_editor.SetSelection(ImGui::TextEditor::Coordinates(), ImGui::TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Dark palette"))
					{
						m_editor.SetPalette(ImGui::TextEditor::GetDarkPalette());
					}

					if (ImGui::MenuItem("Light palette"))
					{
						m_editor.SetPalette(ImGui::TextEditor::GetLightPalette());
					}

					if (ImGui::MenuItem("Retro blue palette"))
					{
						m_editor.SetPalette(ImGui::TextEditor::GetRetroBluePalette());
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_editor.Render("Lua Editor");

			ImGui::End();
		}

		void DevTools::atlas_editor_ui()
		{
			const constexpr short LOADED  = 0;
			const constexpr short CREATED = 1;
			const constexpr short UPDATED = 2;
			const constexpr short SAVED   = 3;

			ImGui::Begin("Texture Atlas Editor", &m_draw_atlas_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Add from JSON"))
			{
				nlohmann::json json = json::parse_from_disk(fs::open_file_dialog("*.json"));

				auto textures = json.at("textures");
				for (const auto& texture : textures)
				{
					SL_HANDLE.atlas()->add(texture.get<std::string>());
				}

				m_atlas_state = LOADED;
			}

			ImGui::SameLine();

			if (ImGui::Button("Add Texture"))
			{
				auto file = fs::open_file_dialog("*.png");
				SL_HANDLE.atlas()->add(file);

				m_atlas_state = LOADED;
			}

			if (ImGui::Button("Create"))
			{
				SL_HANDLE.atlas()->create(*SL_HANDLE.renderer(), *SL_HANDLE.shaderbook()->get("render_to_texture"));

				m_atlas_state = CREATED;
			}

			ImGui::SameLine();

			if (ImGui::Button("Update"))
			{
				SL_HANDLE.atlas()->update(*SL_HANDLE.renderer(), *SL_HANDLE.shaderbook()->get("render_to_texture"));

				m_atlas_state = UPDATED;
			}

			if (ImGui::Button("Save"))
			{
				SL_HANDLE.atlas()->save(fs::save_file_dialog());

				m_atlas_state = SAVED;
			}

			ImGui::SameLine();

			if (ImGui::Button("Save To JSON"))
			{
				nlohmann::json json = "{\"textures\":[]}"_json;
				auto& texture_json  = json["textures"];
				for (const auto& [texture, data] : SL_HANDLE.atlas()->get_tex_data())
				{
					texture_json.push_back(texture);
				}

				json::save_to_disk(fs::save_file_dialog(), json);
				m_atlas_state = SAVED;
			}

			switch (m_atlas_state)
			{
				case LOADED:
					ImGui::Text("Loaded successfully!");
					break;

				case CREATED:
					ImGui::Text("Created successfully!");
					break;

				case UPDATED:
					ImGui::Text("Updated successfully!");
					break;

				case SAVED:
					ImGui::Text("Saved successfully!");
					break;
			}

			ImGui::End();
		}

		void DevTools::entity_ui()
		{
			ImGui::Begin("Entity Manager", &m_draw_entity_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::Text("Input name and press enter to create. Cannot be empty.");
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::InputText("Create", &m_edn_buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				if (!m_edn_buffer.empty())
				{
					m_active_entity     = m_world->create(m_edn_buffer);
					m_entity_debug_name = m_edn_buffer;
					m_edn_buffer        = "";
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Create from JSON"))
			{
				auto file       = fs::open_file_dialog("*.json");
				m_active_entity = m_world->create_from_json(file);
			}

			if (ImGui::BeginCombo("Select Entity", m_entity_debug_name.c_str()))
			{
				for (const auto& [name, id] : m_world->get_debug_name_map())
				{
					const bool selected = (m_entity_debug_name == name);
					if (ImGui::Selectable(name.c_str(), selected))
					{
						m_entity_debug_name = name;
						m_active_entity     = id;
					}

					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			ImGui::Text(fmt::format("Numeric ID: {0}.", m_active_entity).c_str());

			bool enabled = m_world->is_enabled(m_active_entity);
			if (ImGui::Checkbox("Is Enabled?", &enabled))
			{
				if (enabled)
				{
					m_world->set_flag<flags::Enabled>(m_active_entity);
				}
				else
				{
					m_world->unset_flag<flags::Enabled>(m_active_entity);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("[WIP] Serialize"))
			{
				//m_world->serialize(m_active_entity);
				//ImGui::OpenPopup("Entity successfully serialized.");
			}

			if (ImGui::Button("Add Shader"))
			{
				m_world->create_component<graphics::Shader>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Shader"))
			{
				m_world->remove<graphics::Shader>(m_active_entity);
			}

			if (ImGui::Button("Add Sprite"))
			{
				m_world->create_component<graphics::Sprite>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Sprite"))
			{
				m_world->remove<graphics::Sprite>(m_active_entity);
			}

			if (ImGui::Button("Add SpriteBatch"))
			{
				m_world->create_component<graphics::BatchedSprite>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove SpriteBatch"))
			{
				m_world->remove<graphics::BatchedSprite>(m_active_entity);
			}

			if (ImGui::Button("Add Sound"))
			{
				m_world->create_component<audio::Sound>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Sound"))
			{
				m_world->remove<audio::Sound>(m_active_entity);
			}

			if (ImGui::Button("Add Music"))
			{
				m_world->create_component<audio::Music>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Music"))
			{
				m_world->remove<audio::Music>(m_active_entity);
			}

			if (ImGui::Button("Add Animation"))
			{
				m_world->create_component<graphics::AnimatedBatchSprite>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Animation"))
			{
				m_world->remove<graphics::AnimatedBatchSprite>(m_active_entity);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			component_ui(enabled, m_active_entity);

			ImGui::End();
		}

		void DevTools::component_ui(bool enabled, std::uint64_t entity)
		{
			auto [shader, sprite, batch, sound, music, animation, physics] = m_world->get_multi<
			    graphics::Shader,
			    graphics::Sprite,
			    graphics::BatchedSprite,
			    audio::Sound,
			    audio::Music,
			    graphics::AnimatedBatchSprite,
			    physics::BodyWrapper>(entity);

			if (enabled)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Entity is currently: ENABLED.");

				ImGui::Spacing();
				ImGui::Spacing();
			}
			else
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Entity is currently: DISABLED.");

				ImGui::Spacing();
				ImGui::Spacing();
			}

			if (shader != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Shader Component");

				ImGui::Spacing();
				ImGui::Spacing();

				bool confirm = shader->is_loaded();
				if (ImGui::Button("Load"))
				{
					auto vertex = fs::open_file_dialog("*.vs *.vertex *.vert *.v");
					auto frag   = fs::open_file_dialog("*.fs *.fragment *.frag *.f");
					shader->load_path(vertex, frag);
				}

				if (confirm)
				{
					ImGui::Text("Shader has been loaded successfully.");
				}
			}

			if (sprite != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Sprite Component");

				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button("Load Texture"))
				{
					auto file = fs::open_file_dialog("*.png");
					m_sprites_to_create.emplace(sprite, file);
				}

				if (ImGui::Button("Clamp to Border"))
				{
					sprite->clamp_to_border();
				}

				if (ImGui::Button("Clamp to Edge"))
				{
					sprite->clamp_to_edge();
				}

				if (ImGui::Button("Set Mirrored"))
				{
					sprite->set_mirrored();
				}

				if (ImGui::Button("Set Repeated"))
				{
					sprite->set_repeated();
				}

				float opacity = sprite->opacity();
				if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
				{
					sprite->set_opacity(opacity);
				}

				int ansio = sprite->get_aniso_level();
				if (ImGui::SliderInt("Set Ansiotrophy", &ansio, 0, 16))
				{
					sprite->set_anisotropy(ansio);
				}

				if (physics == nullptr)
				{
					glm::vec2 pos = sprite->get_pos();
					if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
					{
						sprite->set_pos(pos.x, pos.y);
					}

					float rotation = sprite->get_rotation();
					if (ImGui::SliderAngle("Rotate", &rotation))
					{
						sprite->rotate(rotation);
					}

					float scale = sprite->get_scale();
					if (ImGui::SliderFloat("Scale", &scale, 1, 10))
					{
						sprite->scale(scale);
					}
				}
			}

			if (batch != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("SpriteBatch Component");

				ImGui::Spacing();
				ImGui::Spacing();

				auto region       = batch->get_region();
				m_batch_region[0] = region.m_x;
				m_batch_region[1] = region.m_y;
				m_batch_region[2] = region.m_width;
				m_batch_region[3] = region.m_height;
				if (ImGui::InputScalarN("Region", ImGuiDataType_Float, &m_batch_region, 4))
				{
					batch->update_region(m_batch_region[0], m_batch_region[1], m_batch_region[2], m_batch_region[3]);
				}

				int z = static_cast<int>(batch->z_level());
				if (ImGui::InputInt("Z-Level", &z, 1, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					batch->set_z_level(z);
				}

				float opacity = batch->opacity();
				if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
				{
					batch->set_opacity(opacity);
				}

				if (physics == nullptr)
				{
					glm::vec2 pos = batch->get_pos();
					if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
					{
						batch->set_pos(pos.x, pos.y);
					}

					float rotation = batch->get_rotation();
					if (ImGui::SliderAngle("Rotate", &rotation))
					{
						batch->rotate(rotation);
					}

					float scale = batch->get_scale();
					if (ImGui::SliderFloat("Scale", &scale, 1, 10))
					{
						batch->scale(scale);
					}
				}
			}

			if (animation != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Animation Component");

				ImGui::Spacing();
				ImGui::Spacing();

				auto region       = animation->get_region();
				m_batch_region[0] = region.m_x;
				m_batch_region[1] = region.m_y;
				m_batch_region[2] = region.m_width;
				m_batch_region[3] = region.m_height;
				if (ImGui::InputScalarN("Region", ImGuiDataType_Float, &m_batch_region, 4))
				{
					animation->update_region(m_batch_region[0], m_batch_region[1], m_batch_region[2], m_batch_region[3]);
				}

				int z = static_cast<int>(animation->z_level());
				if (ImGui::InputInt("Z-Level", &z, 1, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					animation->set_z_level(z);
				}

				float opacity = animation->opacity();
				if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
				{
					animation->set_opacity(opacity);
				}

				if (ImGui::Button("Add Animation"))
				{
					m_add_anim_popup = !m_add_anim_popup;
				}

				if (m_add_anim_popup)
				{
					static std::string s_id                      = "";
					static bool s_loop                           = false;
					static float s_speed                         = 1.0f;
					static std::vector<graphics::Frame> s_frames = {};

					ImGui::BeginPopup("Add Animation", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

					ImGui::InputText("ID", &s_id, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
					ImGui::Checkbox("Is Looping?", &s_loop);
					ImGui::SliderFloat("Speed", &s_speed, 0.1f, 10.0f);

					if (ImGui::Button("Add Frame"))
					{
						static float s_xy[2]          = {0.0f, 0.0f};
						static float s_time_per_frame = 1.0f;

						ImGui::BeginPopup("Add Frame", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

						ImGui::InputScalarN("Texture x/y Coords", ImGuiDataType_Float, &s_xy, 2);
						ImGui::SliderFloat("Time Per Frame", &s_time_per_frame, 0.1f, 10.0f);

						if (ImGui::Button("Add"))
						{
							graphics::Frame frame {s_xy[0], s_xy[1], s_time_per_frame};
							s_frames.emplace_back(frame);

							s_xy[0]          = 0.0f;
							s_xy[1]          = 0.0f;
							s_time_per_frame = 1.0f;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}

					if (ImGui::Button("Add"))
					{
						if (!s_frames.empty())
						{
							animation->add_animation(s_id, s_id, s_loop, static_cast<double>(s_speed), s_frames);

							s_id    = "";
							s_loop  = false;
							s_speed = 1.0f;
							s_frames.clear();

							ImGui::CloseCurrentPopup();
						}
						else
						{
							ImGui::Text("Cannot add an animation without frames.");
						}
					}

					ImGui::EndPopup();
				}

				if (ImGui::BeginCombo("Select Animation", "..."))
				{
					for (const auto& [name, anim] : animation->get_all_anims())
					{
						const bool selected = (m_active_anim == name);
						if (ImGui::Selectable(name.c_str(), selected))
						{
							m_active_anim = name;
						}

						if (selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (!m_active_anim.empty())
				{
					ImGui::Text(fmt::format("Current Anim: {0}.", m_active_anim).c_str());
				}

				if (ImGui::Button("Play Animation"))
				{
					animation->play();
				}

				if (ImGui::Button("Pause Animation"))
				{
					animation->pause();
				}

				if (ImGui::Button("Stop Animation"))
				{
					animation->stop();
				}

				if (physics == nullptr)
				{
					glm::vec2 pos = animation->get_pos();
					if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
					{
						animation->set_pos(pos.x, pos.y);
					}

					float rotation = animation->get_rotation();
					if (ImGui::SliderAngle("Rotate", &rotation))
					{
						animation->rotate(rotation);
					}

					float scale = animation->get_scale();
					if (ImGui::SliderFloat("Scale", &scale, 1, 10))
					{
						animation->scale(scale);
					}
				}
			}

			if (sound != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Sound Component");

				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button("Load"))
				{
					auto file = fs::open_file_dialog();
					sound->load(file);
				}

				if (ImGui::Button("Play"))
				{
					sound->play();
				}

				if (ImGui::Button("Pause"))
				{
					sound->pause();
				}

				if (ImGui::Button("Stop"))
				{
					sound->stop();
				}

				if (ImGui::Button("Rewind"))
				{
					sound->rewind();
				}

				if (ImGui::Checkbox("Is Looping?", &m_sfx_loop))
				{
					sound->set_looping(m_sfx_loop);
				}
			}

			if (music != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Music Component");

				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button("Load"))
				{
					auto file = fs::open_file_dialog();
					music->load(file);
				}

				if (ImGui::Button("Play"))
				{
					music->play();
				}

				if (ImGui::Button("Pause"))
				{
					music->pause();
				}

				if (ImGui::Button("Stop"))
				{
					music->stop();
				}

				if (ImGui::Button("Rewind"))
				{
					music->rewind();
				}

				if (ImGui::Checkbox("Is Looping?", &m_sfx_loop))
				{
					music->set_looping(m_sfx_loop);
				}
			}

			if (physics != nullptr)
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Text("Physics Component");

				ImGui::Spacing();
				ImGui::Spacing();

				if (!physics->m_body->is_rigid())
				{
					ImGui::Text("Body: Kinetic.");

					static float s_hf = 0.0f;
					if (ImGui::InputFloat("Apply Horizontal Force", &s_hf, 0.1, 1, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						auto* kin_body = static_cast<physics::KineticBody*>(physics->m_body.get());
						kin_body->apply_horizontal_force(s_hf);
						s_hf = 0.0f;
					}

					ImGui::SameLine();

					static float s_vf = 0.0f;
					if (ImGui::InputFloat("Apply Vertical Force", &s_vf, 0.1, 1, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						auto* kin_body = static_cast<physics::KineticBody*>(physics->m_body.get());
						kin_body->apply_vertical_force(s_vf);
						s_vf = 0.0f;
					}
				}
				else
				{
					ImGui::Text("Body: Static.");
				}

				ImGui::Spacing();

				ImGui::SliderFloat("Restitution", &physics->m_body->m_restitution, 0.0f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

				ImGui::Spacing();

				ImGui::SliderFloat("Dynamic Friction", &physics->m_body->m_dynamic_friction, 0.0f, 20.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::SliderFloat("Static Friction", &physics->m_body->m_static_friction, 0.0f, 20.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

				ImGui::Spacing();

				ImGui::Text(fmt::format("X Pos: {0}", physics->m_body->get_pos().x).c_str());
				ImGui::SameLine();
				ImGui::Text(fmt::format("Y Pos: {0}", physics->m_body->get_pos().y).c_str());

				ImGui::Spacing();

				ImGui::Text(fmt::format("X Velocity: {0}", physics->m_body->get_vel().x).c_str());
				ImGui::SameLine();
				ImGui::Text(fmt::format("Y Velocity: {0}", physics->m_body->get_vel().y).c_str());

				ImGui::Spacing();

				ImGui::Text(fmt::format("Body Mass: {0}", physics->m_body->mass()).c_str());
			}
		}

		void DevTools::gui_builder_ui()
		{
			ImGui::Begin("GUI Builder", &m_draw_gui_builder_ui, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Create Theme"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("Build GUI"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("Export to JSON"))
			{
			}

			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Add A Widget");
			ImGui::Spacing();

			if (ImGui::Button("Button"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("Image"))
			{
			}

			if (ImGui::Button("Label"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("ProgressBar"))
			{
			}

			if (ImGui::Button("Slider"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("Textbox"))
			{
			}

			if (ImGui::Button("TextInput"))
			{
			}

			ImGui::SameLine();

			if (ImGui::Button("ToggleButton"))
			{
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::End();
		}
	} // namespace tools
} // namespace galaxy