///
/// m_editor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <imgui.h>
#include <pl/Log.hpp>
#include <imgui-SFML.h>
#include <physfs/physfs.h>
#include <tinyfiledialogs.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Editor.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Editor::Editor() noexcept
		:m_window(nullptr)
	{
	}

	Editor::Editor(sf::RenderWindow* window)
		:m_window(nullptr)
	{
		init(window);
	}

	Editor::~Editor()
	{
		shutdown();
	}

	void Editor::init(sf::RenderWindow* window)
	{
		m_window = window;

		// Setup Dear ImGui context and SFML.
		ImGui::SFML::Init(*m_window, true);
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup style.
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();
	}

	void Editor::event(const sf::Event& event)
	{
		ImGui::SFML::ProcessEvent(event);
	}

	void Editor::update(sf::Time& dt)
	{
		ImGui::SFML::Update(*m_window, dt);
	}

	void Editor::render()
	{
		ImGui::SFML::Render();
	}

	void Editor::shutdown()
	{
		ImGui::SFML::Shutdown();
	}

	void Editor::display(bool* restart)
	{
		// Set up stateless variables.
		static bool s_showEditor = false;

		// BEGIN UI.
		ImGui::Begin("Galaxy Editor", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysUseWindowPadding);
		ImGui::SetWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

		// Define the gui layout.
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// This is the code used by the app to restart it.
				if (ImGui::MenuItem("Restart"))
				{
					*restart = true;
					galaxy::ServiceLocator::i().window()->close();
				}

				// Option to close game.
				if (ImGui::MenuItem("Quit"))
				{
					*restart = false;
					galaxy::ServiceLocator::i().window()->close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				// Open script m_editor.
				if (ImGui::MenuItem("Script Editor"))
				{
					s_showEditor = true;
				}

				// Open Lua console.
			//	if (ImGui::MenuItem("Show Console"))
				//{
				//	m_showLuaConsole = true;
			//	}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::SameLine();

		// Push a state.
		//if (ImGui::InputText("Push State", m_stateBuff, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		//{
		//	if (!m_stateBuff.empty())
		//	{
		//			Locator::stateMachine->push(m_stateBuff.c_str());
		//		}
		//	}

		// Pop state.
		//	if (ImGui::Button("Pop State"))
		//	{
		//	Locator::stateMachine->pop();
		//	}

		if (s_showEditor)
		{
			static const std::array<const char*, 2> filter = {
				"*.json", "*.lua"
			};

			auto cpos = m_editor.GetCursorPosition();

			// BEGIN WINDOW.
			ImGui::Begin("Script Editor", &s_showEditor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
			ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

			// BEGIN MENU BAR.
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open", "Ctrl-O"))
					{
						// https://github.com/BalazsJako/ColorTextEditorDemo/blob/master/main.cpp
						// make only json or lua.
						auto openFileName = tinyfd_openFileDialog(
							"Open",
							PHYSFS_getBaseDir(),
							0,
							filter.data(),
							"JSON or Lua.",
							false);

						if (openFileName != nullptr)
						{
							// Convert to C++17 path.
							std::filesystem::path filePath = std::filesystem::path(openFileName);
							m_currentFile = openFileName;

							if (filePath.extension() == ".json")
							{
								m_editor.SetLanguageDefinition(galaxy::getJsonDefinition());
							}
							else if (filePath.extension() == ".lua")
							{
								m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
							}
							else
							{
								PL_LOG(pl::Log::Level::ERROR, "Tried to load unsupported file.");
							}

							std::ifstream text(filePath.string());
							if (!text.fail())
							{
								std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
								m_editor.SetText(str);
							}
							else
							{
								PL_LOG(pl::Log::Level::ERROR, "Failed to read file: " + filePath.string());
							}
							
							text.close();
						}
						else
						{
							PL_LOG(pl::Log::Level::ERROR, "Failed to open file with tfd.");
						}
					}

					if (ImGui::MenuItem("Save", "Ctrl-S"))
					{
						m_editor.GetCursorPosition();
						auto text = m_editor.GetText();
						std::ofstream out(m_currentFile, std::ios::out | std::ios::trunc);
						out << text;
						out.close();
					}

					if (ImGui::MenuItem("Close", "Ctrl-Q"))
					{
						s_showEditor = false;
						m_editor.SetText("");
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = m_editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
						m_editor.SetReadOnly(ro);
					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
						m_editor.Undo();
					if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
						m_editor.Redo();

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
						m_editor.Copy();
					if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_editor.HasSelection()))
						m_editor.Cut();
					if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
						m_editor.Delete();
					if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
						m_editor.Paste();

					ImGui::Separator();

					if (ImGui::MenuItem("Select all", nullptr, nullptr))
						m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Dark palette"))
						m_editor.SetPalette(TextEditor::GetDarkPalette());
					if (ImGui::MenuItem("Light palette"))
						m_editor.SetPalette(TextEditor::GetLightPalette());
					if (ImGui::MenuItem("Retro blue palette"))
						m_editor.SetPalette(TextEditor::GetRetroBluePalette());
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
				// END MENU BAR.
			}
			
			m_editor.Render("TextEditor");

			ImGui::End();
			// END WINDOW.
		}

		//if (m_showLuaConsole)
		//{
			// Create and show console.
			//static ImGui::Console console;
			//console.Draw("Lua Console", &m_showLuaConsole);
		//}

		ImGui::End();
		// END UI.
	}
}