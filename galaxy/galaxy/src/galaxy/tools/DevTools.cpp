///
/// DevTools.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <imgui/imgui_stdlib.h>
#include <imgui/impl/imgui_impl_glfw.h>
#include <imgui/impl/imgui_impl_opengl3.h>
#include <imgui/addons/ToggleButton.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/scripting/JSONUtils.hpp>
#include <qs/graphics/TextureAtlas.hpp>
#include <galaxy/components/All.hpp>

#include "DevTools.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	DevTools::DevTools()
	    : m_world {nullptr}, m_window {nullptr}, m_draw_json_editor {false}, m_draw_script_editor {false}, m_draw_atlas_editor {false}, m_draw_entity_editor {false}, m_draw_lua_console {false}, m_atlas_state {-1}, m_show_entity_create {false}, m_entity_debug_name {"..."}, m_active_entity {0}, m_edn_buffer {""}
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
		ImGui::StyleColorsClassic();
		// clang-format on

		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		m_editor.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::Lua());
	}

	void DevTools::prepare()
	{
		if (!m_sprites_to_create.empty())
		{
			for (auto& [sprite, path] : m_sprites_to_create)
			{
				sprite->load(path);
				sprite->create<qs::BufferDynamic>();
			}

			m_sprites_to_create.clear();
		}

		start();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					m_window->close();
				}

				if (ImGui::MenuItem("Restart"))
				{
					SL_HANDLE.m_restart = true;
					m_window->close();
				}

				ImGui::EndMenu();
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

			ImGui::EndMainMenuBar();
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

	void DevTools::json_ui()
	{
		ImGui::Begin("JSON Editor", &m_draw_json_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Visual JSON editor.");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("create_new", ImGuiPopupFlags_NoOpenOverExistingPopup);
		}

		m_json_editor.create_new();
		ImGui::SameLine();

		if (ImGui::Button("Open"))
		{
			auto fp = galaxy::FileSystem::open_file_dialog("*.json");
			m_json_editor.load_file(fp);
		}

		ImGui::SameLine();

		if (ImGui::Button("Save"))
		{
			if (m_json_editor.is_loaded())
			{
				auto fp = galaxy::FileSystem::save_file_dialog();
				m_json_editor.save(fp);
			}
		}

		m_json_editor.parse_and_display();

		ImGui::End();
	}

	void DevTools::script_editor_ui()
	{
		const auto cpos = m_editor.GetCursorPosition();

		ImGui::Begin("Lua Editor", &m_draw_script_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					auto fp = std::filesystem::path {galaxy::FileSystem::open_file_dialog("*.lua")};
					std::ifstream text(fp.string(), std::ifstream::in);
					if (text.good())
					{
						std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
						m_editor.SetText(str);
					}
					else
					{
						PL_LOG(PL_ERROR, "Failed to read file: {0}.", fp.string());
					}

					text.close();
				}

				if (ImGui::MenuItem("Save"))
				{
					auto fp   = std::filesystem::path {galaxy::FileSystem::save_file_dialog()};
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

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
				{
					m_editor.Undo();
				}

				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
				{
					m_editor.Redo();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
				{
					m_editor.Copy();
				}

				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Cut();
				}

				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Delete();
				}

				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
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
			nlohmann::json json = galaxy::json::parse_from_disk(galaxy::FileSystem::open_file_dialog("*.json"));

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
			auto file = galaxy::FileSystem::open_file_dialog("*.png");
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
			SL_HANDLE.atlas()->save(galaxy::FileSystem::save_file_dialog());

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

			galaxy::json::save_to_disk(galaxy::FileSystem::save_file_dialog(), json);
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
			auto file       = galaxy::FileSystem::open_file_dialog("*.json");
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

		if (m_world->validate(m_active_entity) && m_world->has(m_active_entity))
		{
			ImGui::Text(fmt::format("Numeric ID: {0}.", m_active_entity).c_str());

			bool enabled = m_world->is_enabled<galaxy::EnabledComponent>(m_active_entity);
			if (ImGui::Checkbox("Is Enabled?", &enabled))
			{
				if (enabled)
				{
					m_world->create_component<galaxy::EnabledComponent>(m_active_entity);
				}
				else
				{
					m_world->remove<galaxy::EnabledComponent>(m_active_entity);
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
				m_world->create_component<galaxy::ShaderComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Shader"))
			{
				m_world->remove<galaxy::ShaderComponent>(m_active_entity);
			}

			if (ImGui::Button("Add Sprite"))
			{
				m_world->create_component<galaxy::SpriteComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Sprite"))
			{
				m_world->remove<galaxy::SpriteComponent>(m_active_entity);
			}

			if (ImGui::Button("Add SpriteBatch"))
			{
				m_world->create_component<galaxy::SpriteBatchComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove SpriteBatch"))
			{
				m_world->remove<galaxy::SpriteBatchComponent>(m_active_entity);
			}

			if (ImGui::Button("Add Sound"))
			{
				m_world->create_component<galaxy::SoundComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Sound"))
			{
				m_world->remove<galaxy::SoundComponent>(m_active_entity);
			}

			if (ImGui::Button("Add Music"))
			{
				m_world->create_component<galaxy::MusicComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Music"))
			{
				m_world->remove<galaxy::MusicComponent>(m_active_entity);
			}

			if (ImGui::Button("Add Animation"))
			{
				m_world->create_component<galaxy::AnimationComponent>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Animation"))
			{
				m_world->remove<galaxy::AnimationComponent>(m_active_entity);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			component_ui(enabled, m_active_entity);
		}

		ImGui::End();
	}

	void DevTools::component_ui(bool enabled, std::uint32_t entity)
	{
		auto [shader, sprite, batch, sound, music, animation] = m_world->get_multi<
		    galaxy::ShaderComponent,
		    galaxy::SpriteComponent,
		    galaxy::SpriteBatchComponent,
		    galaxy::SoundComponent,
		    galaxy::MusicComponent,
		    galaxy::AnimationComponent>(entity);

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

			bool confirm = shader->m_shader.is_loaded();
			if (ImGui::Button("Load"))
			{
				auto vertex = galaxy::FileSystem::open_file_dialog("*.vs *.vertex *.vert *.v");
				auto frag   = galaxy::FileSystem::open_file_dialog("*.fs *.fragment *.frag *.f");
				shader->m_shader.load_path(vertex, frag);
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
				auto file = galaxy::FileSystem::open_file_dialog("*.png");
				m_sprites_to_create.emplace(&sprite->m_sprite, file);
			}

			if (ImGui::Button("Clamp to Border"))
			{
				sprite->m_sprite.clamp_to_border();
			}

			if (ImGui::Button("Clamp to Edge"))
			{
				sprite->m_sprite.clamp_to_edge();
			}

			if (ImGui::Button("Set Mirrored"))
			{
				sprite->m_sprite.set_mirrored();
			}

			if (ImGui::Button("Set Repeated"))
			{
				sprite->m_sprite.set_repeated();
			}

			float opacity = sprite->m_sprite.opacity();
			if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
			{
				sprite->m_sprite.set_opacity(opacity);
			}

			int ansio = sprite->m_sprite.get_aniso_level();
			if (ImGui::SliderInt("Set Ansiotrophy", &ansio, 0, 16))
			{
				sprite->m_sprite.set_anisotropy(ansio);
			}

			glm::vec2 pos = sprite->m_sprite.get_pos();
			if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
			{
				sprite->m_sprite.set_pos(pos.x, pos.y);
			}

			float rotation = sprite->m_sprite.get_rotation();
			if (ImGui::SliderAngle("Rotate", &rotation))
			{
				sprite->m_sprite.rotate(rotation);
			}

			float scale = sprite->m_sprite.get_scale();
			if (ImGui::SliderFloat("Scale", &scale, 1, 10))
			{
				sprite->m_sprite.scale(scale);
			}
		}

		if (batch != nullptr)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("SpriteBatch Component");

			ImGui::Spacing();
			ImGui::Spacing();

			auto region       = batch->m_bs.get_region();
			m_batch_region[0] = region.m_x;
			m_batch_region[1] = region.m_y;
			m_batch_region[2] = region.m_width;
			m_batch_region[3] = region.m_height;
			if (ImGui::InputScalarN("Region", ImGuiDataType_Float, &m_batch_region, 4))
			{
				batch->m_bs.update_region(m_batch_region[0], m_batch_region[1], m_batch_region[2], m_batch_region[3]);
			}

			int z = static_cast<int>(batch->m_bs.z_level());
			if (ImGui::InputInt("Z-Level", &z, 1, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				batch->m_bs.set_z_level(z);
			}

			float opacity = batch->m_bs.opacity();
			if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
			{
				batch->m_bs.set_opacity(opacity);
			}

			glm::vec2 pos = batch->m_bs.get_pos();
			if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
			{
				batch->m_bs.set_pos(pos.x, pos.y);
			}

			float rotation = batch->m_bs.get_rotation();
			if (ImGui::SliderAngle("Rotate", &rotation))
			{
				batch->m_bs.rotate(rotation);
			}

			float scale = batch->m_bs.get_scale();
			if (ImGui::SliderFloat("Scale", &scale, 1, 10))
			{
				batch->m_bs.scale(scale);
			}
		}

		if (animation != nullptr)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("Animation Component");

			ImGui::Spacing();
			ImGui::Spacing();

			auto region       = animation->m_abs.get_region();
			m_batch_region[0] = region.m_x;
			m_batch_region[1] = region.m_y;
			m_batch_region[2] = region.m_width;
			m_batch_region[3] = region.m_height;
			if (ImGui::InputScalarN("Region", ImGuiDataType_Float, &m_batch_region, 4))
			{
				animation->m_abs.update_region(m_batch_region[0], m_batch_region[1], m_batch_region[2], m_batch_region[3]);
			}

			int z = static_cast<int>(animation->m_abs.z_level());
			if (ImGui::InputInt("Z-Level", &z, 1, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				animation->m_abs.set_z_level(z);
			}

			float opacity = animation->m_abs.opacity();
			if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
			{
				animation->m_abs.set_opacity(opacity);
			}

			if (ImGui::Button("Add Animation"))
			{
				m_add_anim_popup = !m_add_anim_popup;
			}

			if (m_add_anim_popup)
			{
				static std::string s_id                = "";
				static bool s_loop                     = false;
				static float s_speed                   = 1.0f;
				static std::vector<qs::Frame> s_frames = {};

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
						qs::Frame frame {s_xy[0], s_xy[1], s_time_per_frame};
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
						animation->m_abs.add_animation(s_id, s_id, s_loop, static_cast<double>(s_speed), s_frames);

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
				for (const auto& [name, anim] : animation->m_abs.get_all_anims())
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
				animation->m_abs.play();
			}

			if (ImGui::Button("Pause Animation"))
			{
				animation->m_abs.pause();
			}

			if (ImGui::Button("Stop Animation"))
			{
				animation->m_abs.stop();
			}

			glm::vec2 pos = animation->m_abs.get_pos();
			if (ImGui::InputScalarN("Pos", ImGuiDataType_Float, &pos, 2))
			{
				animation->m_abs.set_pos(pos.x, pos.y);
			}

			float rotation = animation->m_abs.get_rotation();
			if (ImGui::SliderAngle("Rotate", &rotation))
			{
				animation->m_abs.rotate(rotation);
			}

			float scale = animation->m_abs.get_scale();
			if (ImGui::SliderFloat("Scale", &scale, 1, 10))
			{
				animation->m_abs.scale(scale);
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
				auto file = galaxy::FileSystem::open_file_dialog();
				sound->m_sound.load(file);
			}

			if (ImGui::Button("Play"))
			{
				sound->m_sound.play();
			}

			if (ImGui::Button("Pause"))
			{
				sound->m_sound.pause();
			}

			if (ImGui::Button("Stop"))
			{
				sound->m_sound.stop();
			}

			if (ImGui::Button("Rewind"))
			{
				sound->m_sound.rewind();
			}

			if (ImGui::Checkbox("Is Looping?", &m_sfx_loop))
			{
				sound->m_sound.set_looping(m_sfx_loop);
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
				auto file = galaxy::FileSystem::open_file_dialog();
				music->m_music.load(file);
			}

			if (ImGui::Button("Play"))
			{
				music->m_music.play();
			}

			if (ImGui::Button("Pause"))
			{
				music->m_music.pause();
			}

			if (ImGui::Button("Stop"))
			{
				music->m_music.stop();
			}

			if (ImGui::Button("Rewind"))
			{
				music->m_music.rewind();
			}

			if (ImGui::Checkbox("Is Looping?", &m_sfx_loop))
			{
				music->m_music.set_looping(m_sfx_loop);
			}
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
} // namespace galaxy