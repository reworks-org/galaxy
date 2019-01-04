///
/// DebugInterface.cpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifdef _DEBUG

#include <physfs.h>

#include "sl/core/World.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/libs/imgui/imgui_stl.h"
#include "sl/libs/imgui/imgui_console.h"
#include "sl/libs/imgui/imgui_impl_allegro5.h"

#include "DebugInterface.hpp"

namespace sl
{
	DebugInterface::DebugInterface(const std::string& scriptFolderPath, ALLEGRO_DISPLAY* display)
	:m_scriptFolderPath(scriptFolderPath)
	{
		// Set up imgui context from allegro's display.
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplAllegro5_Init(display);

		// Setup style
		ImGui::StyleColorsDark();

		// Then set up the text editor for lua script editing.
		m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		m_editor.SetPalette(TextEditor::GetDarkPalette());
	}

	DebugInterface::~DebugInterface()
	{
		// Clean up all the imgui data.
		ImGui_ImplAllegro5_Shutdown();
		ImGui::DestroyContext();
	}

	void DebugInterface::event(ALLEGRO_EVENT* event)
	{
		// If not disabled, process gui events.
		ImGui_ImplAllegro5_ProcessEvent(event);
	}

	void DebugInterface::newFrame()
	{
		// If not disabled, create a new frame for drawing widgets to.
		ImGui_ImplAllegro5_NewFrame();
		ImGui::NewFrame();
	}

	void DebugInterface::render()
	{
		// If not disabled, draw the imgui window.
		ImGui::Render();
		ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	}

	void DebugInterface::displayMenu(bool* restart)
	{
		// Set up the variables required to use the gui.
		// Declared static otherwise data would be reset / lost when losing scope.
		static bool s_showLuaConsole = false;
		static bool s_showScriptEditor = false;
		static std::string s_buff = "";
		static std::string s_stateBuff = "";

		// Set up the gui.
		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

		// Define the gui layout.
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// This is the code used by the app to restart it.
				if (ImGui::MenuItem("Restart Game"))
				{
					*restart = true;
					Locator::window->close();
				}

				// Option to close game.
				if (ImGui::MenuItem("Quit Game"))
				{
					Locator::window->close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				// Open script editor.
				if (ImGui::MenuItem("Script Editor"))
				{
					s_showScriptEditor = true;
				}

				// Open Lua console.
				if (ImGui::MenuItem("Show Console"))
				{
					s_showLuaConsole = true;
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		// Push a state.
		if (ImGui::InputText("Push State", s_stateBuff, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!s_stateBuff.empty())
			{
				Locator::stateMachine->push(s_stateBuff.c_str());
			}
		}

		ImGui::SameLine();
		
		// Pop state.
		if (ImGui::Button("Pop State"))
		{
			Locator::stateMachine->pop();
		}

		// Input the name of a script in the VFS to create an entity from.
		if (ImGui::InputText("Create Entity from Script", s_buff, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!s_buff.empty())
			{
				Locator::world->createEntity(s_buff);
			}
		}

		if (s_showScriptEditor)
		{
			// Set up variables.
			static int s_index = 0;
			static bool s_showFilesToLoad = false;
			static std::string s_currentScript = "";
			static std::vector<std::string> s_files;

			// Open window, ensuring that there are scrollbars avaliable if text is bigger than window.
			ImGui::Begin("Script Editor", &s_showScriptEditor, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
			ImGui::SetWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver); // 800, 600

			// Define the gui layout.
			// This section is pretty self-explainatory.
			auto cpos = m_editor.GetCursorPosition();
			if (ImGui::BeginMenuBar())
			{
				// All the file management functions.
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						s_showFilesToLoad = true;
					}

					if (ImGui::MenuItem("Save"))
					{
						if (!s_currentScript.empty())
						{
							Locator::virtualFS->writeToFile(s_currentScript, m_editor.GetText().c_str());
						}
					}

					if (ImGui::MenuItem("Close"))
					{
						s_showFilesToLoad = false;
						s_showScriptEditor = false;
						s_currentScript = "";
						m_editor.SetText(" ");
					}

					ImGui::EndMenu();
				}

				// All the editing functions.
				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = m_editor.IsReadOnly();

					if (ImGui::MenuItem("Undo", nullptr, !ro && m_editor.CanUndo()))
					{
						m_editor.Undo();
					}

					if (ImGui::MenuItem("Redo", nullptr, !ro && m_editor.CanRedo()))
					{
						m_editor.Redo();
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", nullptr, m_editor.HasSelection()))
					{
						m_editor.Copy();
					}

					if (ImGui::MenuItem("Cut", nullptr, !ro && m_editor.HasSelection()))
					{
						m_editor.Cut();
					}

					if (ImGui::MenuItem("Delete", nullptr, !ro && m_editor.HasSelection()))
					{
						m_editor.Delete();
					}

					if (ImGui::MenuItem("Paste", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
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

				ImGui::EndMenuBar();
			}
			// End layout definition.

			if (s_showFilesToLoad)
			{
				// Make sure variables are empty.
				s_index = 0;
				s_files.clear();
				s_currentScript = "";

				// Loop over scripts folder getting all script names using physfs.
				char** efl = PHYSFS_enumerateFiles(m_scriptFolderPath.c_str());
				if (!efl)
				{
					LOG_S(WARNING) << "Could not load scripts (or there are no scripts). Last Physfs error: " << PHYSFS_getLastError();
				}
				else
				{
					for (char** i = efl; *i != NULL; i++)
					{
						// Make sure the path is added so the script can be saved to the overrides folder.
						s_files.emplace_back(m_scriptFolderPath + std::string(*i));
					}
				}

				// Makes sure we are not freeing nothing.
				if (efl)
				{
					PHYSFS_freeList(efl);
				}

				ImGui::Begin("Select Script", &s_showFilesToLoad, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				// Then a listbox is used to display all the loaded scripts.
				if (ImGui::ListBox("", &s_index, s_files))
				{
					// Simply load the contents of the script using the vfs.
					s_currentScript = s_files[s_index];
					std::string loadedText = Locator::virtualFS->openAsString(s_currentScript);
					
					if (loadedText.empty())
					{
						LOG_S(WARNING) << "Script file was empty! Or Loading failed. Check for other errors in log. Script: " << s_currentScript;
					}
					else
					{
						// Then add the text to the editor.
						m_editor.SetText(loadedText);
					}

					s_showFilesToLoad = false;
				}

				ImGui::End();
			}

			// Display info about the script to the script editor window, like linecount and language.
			ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_editor.GetTotalLines(),
				m_editor.IsOverwrite() ? "Ovr" : "Ins",
				m_editor.CanUndo() ? "*" : " ",
				m_editor.GetLanguageDefinition().mName.c_str(), s_currentScript.c_str());

			ImGui::Spacing();

			m_editor.Render("Code Editor");

			ImGui::End();
		}

		if (s_showLuaConsole)
		{
			// Create and show console.
			static ImGui::Console console;
			console.Draw("Lua Console", &s_showLuaConsole);
		}

		ImGui::End();
	}
}

#endif