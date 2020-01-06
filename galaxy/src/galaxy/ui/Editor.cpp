///
/// Editor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define ZEP_SINGLE_HEADER_BUILD
#define ZEP_FEATURE_THREADS
#define ZEP_FEATURE_CPP_FILE_SYSTEM
#include <zep.h>
#include <zep/theme.h>
#include <zep/window.h>
#include <zep/buffer.h>
#include <zep/tab_window.h>
#include <zep/mode_standard.h>
#include <zep/imgui/display_imgui.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Editor.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Editor::Editor() noexcept
		:m_window(nullptr), m_zepEditor(Zep::ZepPath())
	{
	}

	Editor::Editor(sf::RenderWindow* window)
		:m_window(nullptr), m_zepEditor(Zep::ZepPath())
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

		// Setup style.
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.1f, 0.12f, 0.95f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));

		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1024, 768), ImGuiCond_FirstUseEver);
	}

	void Editor::event(const sf::Event& event)
	{
		ImGui::SFML::ProcessEvent(event);
	}

	void Editor::update(sf::Clock& clock)
	{
		ImGui::SFML::Update(*m_window, clock.restart());
	}

	void Editor::render()
	{
		ImGui::SFML::Render(*m_window);
	}

	void Editor::shutdown()
	{
		ImGui::SFML::Shutdown();
	}

	void Editor::display(bool* restart)
	{
		// Set up stateless variables.
		static bool s_showEditor = false;

		ImGui::Begin("Galaxy Editor", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

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
				// Open script editor.
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

		// Push a state.
		//if (ImGui::InputText("Push State", m_stateBuff, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		//{
		//	if (!m_stateBuff.empty())
		//	{
	//			Locator::stateMachine->push(m_stateBuff.c_str());
	//		}
	//	}

		ImGui::SameLine();

		// Pop state.
	//	if (ImGui::Button("Pop State"))
	//	{
		//	Locator::stateMachine->pop();
	//	}

		// Input the name of a script in the VFS to create an entity from.
	///	if (ImGui::InputText("Create Entity from Script", m_buff, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
	//	{
		//	if (!m_buff.empty())
	//		{
	//			Locator::world->createEntity(m_buff);
	//		}
	//	}

		if (s_showEditor)
		{
			// Open window, ensuring that there are scrollbars avaliable if text is bigger than window.
			ImGui::Begin("Script Editor", &s_showEditor, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
			ImGui::SetWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver); // 800, 600
			m_zepEditor.SetGlobalMode(Zep::ZepMode_Standard::StaticName());

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Settings"))
				{
					if (ImGui::BeginMenu("Theme"))
					{
						bool enabledDark = m_zepEditor.GetTheme().GetThemeType() == Zep::ThemeType::Dark ? true : false;
						bool enabledLight = !enabledDark;

						if (ImGui::MenuItem("Dark", "", &enabledDark))
						{
							m_zepEditor.GetTheme().SetThemeType(Zep::ThemeType::Dark);
						}
						else if (ImGui::MenuItem("Light", "", &enabledLight))
						{
							m_zepEditor.GetTheme().SetThemeType(Zep::ThemeType::Light);
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Window"))
				{
					auto pTabWindow = m_zepEditor.GetActiveTabWindow();
					if (ImGui::MenuItem("Horizontal Split"))
					{
						pTabWindow->AddWindow(&pTabWindow->GetActiveWindow()->GetBuffer(), pTabWindow->GetActiveWindow(), false);
					}
					else if (ImGui::MenuItem("Vertical Split"))
					{
						pTabWindow->AddWindow(&pTabWindow->GetActiveWindow()->GetBuffer(), pTabWindow->GetActiveWindow(), true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			// End layout definition.

			m_zepEditor.RefreshRequired();        // Currently required once per frame
			m_zepEditor.Display();

			if (ImGui::IsWindowFocused())
			{
				m_zepEditor.HandleInput();
			}
		
			ImGui::End();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(1);
		}
			/*
			if (m_showFilesToLoad)
			{
				// Make sure variables are empty.
				m_index = 0;
				m_files.clear();
				m_currentScript = "";

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
						m_files.emplace_back(m_scriptFolderPath + std::string(*i));
					}
				}

				// Makes sure we are not freeing nothing.
				if (efl)
				{
					PHYSFS_freeList(efl);
				}

				ImGui::Begin("Select Script", &m_showFilesToLoad, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				// Then a listbox is used to display all the loaded scripts.
				if (ImGui::ListBox("", &m_index, m_files))
				{
					// Simply load the contents of the script using the vfs.
					m_currentScript = m_files[m_index];
					std::string loadedText = Locator::virtualFS->openAsString(m_currentScript);

					if (loadedText.empty())
					{
						LOG_S(WARNING) << "Script file was empty! Or Loading failed. Check for other errors in log. Script: " << m_currentScript;
					}
					else
					{
						// Then add the text to the editor.
						m_editor.SetText(loadedText);
					}

					m_showFilesToLoad = false;
				}

				ImGui::End();
			}

			// Display info about the script to the script editor window, like linecount and language.
			ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_editor.GetTotalLines(),
				m_editor.IsOverwrite() ? "Ovr" : "Ins",
				m_editor.CanUndo() ? "*" : " ",
				m_editor.GetLanguageDefinition().mName.c_str(), m_currentScript.c_str());

			ImGui::Spacing();

			m_editor.Render("Code Editor");

			ImGui::End();
		}
		*/

		//if (m_showLuaConsole)
		//{
			// Create and show console.
			//static ImGui::Console console;
			//console.Draw("Lua Console", &m_showLuaConsole);
		//}

		ImGui::End();
	}
}