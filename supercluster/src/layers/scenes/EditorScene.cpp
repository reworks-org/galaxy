///
/// EditorScene.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/BatchedSprite.hpp>
#include <galaxy/components/Circle.hpp>
#include <galaxy/components/Line.hpp>
#include <galaxy/components/Point.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/ShaderID.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/World.hpp>

#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/platform/Platform.hpp>
#include <galaxy/res/TextureAtlas.hpp>
#include <galaxy/scripting/JSONUtils.hpp>
#include <galaxy/systems/RenderSystem.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "../../editor/Theme.hpp"

#include "EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	EditorScene::EditorScene()
	    : m_window {SL_HANDLE.window()}, m_process {nullptr}, m_draw_main {true}, m_draw_demo {false}, m_draw_json_editor {false}, m_draw_script_editor {false}, m_draw_entity_editor {false}, m_draw_lua_console {false}, m_draw_gui_builder_ui {false}, m_visible_tools {true}, m_show_entity_create {false}, m_entity_debug_name {"..."}, m_active_entity {0}, m_edn_buffer {""}
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		// clang-format off
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;
		io.ConfigDockingWithShift = true;
		// clang-format on

		editor::theme::visual_dark();
		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);

		const constexpr char* gl_version = "#version 450 core";
		ImGui_ImplOpenGL3_Init(gl_version);

		m_editor.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::Lua());
		m_world.create_system<systems::RenderSystem>();
	}

	EditorScene::~EditorScene()
	{
		m_window  = nullptr;
		m_process = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorScene::events()
	{
		if (m_window->key_pressed(input::Keys::GRAVE))
		{
			m_visible_tools = !m_visible_tools;
		}
	}

	void EditorScene::update(const double dt)
	{
		m_camera.update(dt);
		m_world.update(dt);
	}

	void EditorScene::pre_render()
	{
		if (!m_sprites_to_create.empty())
		{
			for (auto& [sprite, path] : m_sprites_to_create)
			{
				sprite->load(path);
				sprite->create();
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

		ImGui::DockSpace(ImGui::GetID("EditorScene_Dockspace_01"), {0.0f, 0.0f}, dockspace_flags);

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

				if (ImGui::MenuItem("Restart"))
				{
					SL_HANDLE.m_restart = true;
					m_window->close();
				}

				if (ImGui::MenuItem("Exit"))
				{
					platform::close_process(m_process);
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

			if (ImGui::MenuItem("Open Tiled"))
			{
				m_process = platform::run_process("tools/tiled/tiled.exe");
			}

			ImGui::EndMenuBar();
		}

		if (m_draw_demo)
		{
			ImGui::ShowDemoWindow(&m_draw_demo);
		}

		if (m_draw_json_editor)
		{
			json_ui();
		}

		if (m_draw_script_editor)
		{
			script_editor_ui();
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

	void EditorScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorScene::start()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorScene::end()
	{
		ImGui::Render();
	}

	void EditorScene::json_ui()
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
			const auto path = SL_HANDLE.vfs()->show_open_dialog("*.json");
			m_json_editor.load_file(path);
		}

		ImGui::SameLine();

		if (ImGui::Button("Save"))
		{
			if (m_json_editor.is_loaded())
			{
				const auto path = SL_HANDLE.vfs()->show_save_dialog();
				m_json_editor.save(path);
			}
		}

		ImGui::Separator();
		ImGui::Spacing();

		m_json_editor.parse_and_display();

		ImGui::End();
	}

	void EditorScene::script_editor_ui()
	{
		ImGui::Begin("Lua Editor", &m_draw_script_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					const auto code = SL_HANDLE.vfs()->open_with_dialog("*.lua");
					m_editor.SetText(code);
				}

				if (ImGui::MenuItem("Save"))
				{
					SL_HANDLE.vfs()->save_with_dialog(m_editor.GetText());
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

	void EditorScene::entity_ui()
	{
		ImGui::Begin("Entity Manager", &m_draw_entity_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Input name and press enter to create. Cannot be empty.");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::InputText("Create", &m_edn_buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			if (!m_edn_buffer.empty())
			{
				m_active_entity     = m_world.create(m_edn_buffer);
				m_entity_debug_name = m_edn_buffer;
				m_edn_buffer        = "";
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Create from JSON"))
		{
			const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
			m_active_entity = m_world.create_from_json(file);
		}

		if (ImGui::BeginCombo("Select Entity", m_entity_debug_name.c_str()))
		{
			for (const auto& [name, id] : m_world.get_debug_name_map())
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

		if (m_world.has(m_active_entity))
		{
			bool enabled = m_world.is_enabled(m_active_entity);
			if (ImGui::Checkbox("Is Enabled?", &enabled))
			{
				if (enabled)
				{
					m_world.enable(m_active_entity);
				}
				else
				{
					m_world.disable(m_active_entity);
				}
			}

			if (ImGui::Button("Add BatchedSprite"))
			{
				m_world.create_component<components::BatchedSprite>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove BatchedSprite"))
			{
				m_world.remove<components::BatchedSprite>(m_active_entity);
			}

			if (ImGui::Button("Add Circle"))
			{
				m_world.create_component<components::Circle>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Circle"))
			{
				m_world.remove<components::Circle>(m_active_entity);
			}

			if (ImGui::Button("Add Line"))
			{
				m_world.create_component<components::Line>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Line"))
			{
				m_world.remove<components::Line>(m_active_entity);
			}

			if (ImGui::Button("Add Point"))
			{
				m_world.create_component<components::Point>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Point"))
			{
				m_world.remove<components::Point>(m_active_entity);
			}

			if (ImGui::Button("Add Renderable"))
			{
				m_world.create_component<components::Renderable>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Renderable"))
			{
				m_world.remove<components::Renderable>(m_active_entity);
			}

			if (ImGui::Button("Add ShaderID"))
			{
				m_world.create_component<components::ShaderID>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove ShaderID"))
			{
				m_world.remove<components::ShaderID>(m_active_entity);
			}

			if (ImGui::Button("Add Sprite"))
			{
				m_world.create_component<components::Sprite>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Sprite"))
			{
				m_world.remove<components::Sprite>(m_active_entity);
			}

			if (ImGui::Button("Add Text"))
			{
				m_world.create_component<components::Text>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Text"))
			{
				m_world.remove<components::Text>(m_active_entity);
			}

			if (ImGui::Button("Add Transform"))
			{
				m_world.create_component<components::Transform>(m_active_entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Transform"))
			{
				m_world.remove<components::Transform>(m_active_entity);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			if (enabled)
			{
				component_ui(enabled, m_active_entity);
			}
		}

		ImGui::End();
	}

	void EditorScene::component_ui(bool enabled, std::uint64_t entity)
	{
		auto [batchedsprite, circle, line, point, renderable, shaderid, sprite, text, transform] = m_world.get_multi<
		    components::BatchedSprite,
		    components::Circle,
		    components::Line,
		    components::Point,
		    components::Renderable,
		    components::ShaderID,
		    components::Sprite,
		    components::Text,
		    components::Transform>(entity);
	}

	void EditorScene::gui_builder_ui()
	{
		ImGui::Begin("GUI Builder", &m_draw_gui_builder_ui, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::End();
	}
} // namespace sc