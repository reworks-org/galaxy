///
/// ImGuiLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/scripting/JSONDefinition.hpp>

#include "ImGuiLayer.hpp"

namespace sc
{
	ImGuiLayer::ImGuiLayer()
	    : m_world {nullptr}, m_window {nullptr}, m_show_script_editor {false}
	{
		// clang-format off
		set_name("imgui_layer");
		m_window = SL_HANDLE.window();
		m_world  = SL_HANDLE.world();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();

		ImGui_ImplGlfw_InitForOpenGL(m_window->gl_window(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
		// clang-format on
	}

	ImGuiLayer::~ImGuiLayer()
	{
		m_world  = nullptr;
		m_window = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::events()
	{
	}

	void ImGuiLayer::update(const double dt)
	{
	}

	void ImGuiLayer::render(qs::Camera& camera)
	{
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

			if (ImGui::BeginMenu("Editors"))
			{
				if (ImGui::MenuItem("Script Editor"))
				{
					m_show_script_editor = !m_show_script_editor;
				}

				if (ImGui::MenuItem("Texture Atlas Editor"))
				{
					//m_showTAEUI = !m_showTAEUI;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("ECS"))
			{
				if (ImGui::MenuItem("Entity Manager"))
				{
					//m_showEUI = !m_showEUI;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Lua Console"))
			{
				//m_drawConsole = !m_drawConsole;
			}

			ImGui::EndMainMenuBar();
		}

		if (m_show_script_editor)
		{
			script_editor_ui();
		}

		/*
		if (m_showEUI)
		{
			entityUI();
		}

		if (m_showTEUI)
		{
			scriptEditorUI();
		}

		if (m_showTAEUI)
		{
			textureAtlasEditor();
		}

		if (m_drawConsole)
		{
			m_console.draw(&m_drawConsole);
		}
*/
		end();
	}

	void ImGuiLayer::start()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::script_editor_ui()
	{
		const auto cpos = m_editor.GetCursorPosition();

		ImGui::Begin("Script Editor", &m_show_script_editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					auto fp = galaxy::FileSystem::open_file_dialog("*.json *.lua");
					if (fp.extension() == ".json")
					{
						m_editor.SetLanguageDefinition(galaxy::get_json_definition());
					}
					else if (fp.extension() == ".lua")
					{
						m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
					}

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
					auto fp   = galaxy::FileSystem::save_file_dialog();
					auto text = m_editor.GetText();

					std::ofstream out(fp.string(), std::ios::out | std::ios::trunc);
					out << text;
					out.close();
				}

				if (ImGui::MenuItem("Close"))
				{
					m_show_script_editor = false;
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
					m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
				{
					m_editor.SetPalette(TextEditor::GetDarkPalette());
				}

				if (ImGui::MenuItem("Light palette"))
				{
					m_editor.SetPalette(TextEditor::GetLightPalette());
				}

				if (ImGui::MenuItem("Retro blue palette"))
				{
					m_editor.SetPalette(TextEditor::GetRetroBluePalette());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_editor.Render("Script Editor");

		ImGui::End();
	}
} // namespace sc