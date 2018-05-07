///
/// DebugInterface.cpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef NDEBUG

#include <physfs.h>
#include <allegro5/events.h>
#include <allegro5/display.h>

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/imgui/imgui_impl_a5.h"

#include "DebugInterface.hpp"

///
/// Based off of example in imgui demo, under same license.
///
struct ExampleAppConsole
{
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	sol::state* luaState;

	ExampleAppConsole(sol::state* state)
	{
		luaState = state;
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;
	}

	~ExampleAppConsole()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

	void    ClearLog()
	{
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
		ScrollToBottom = true;
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
		ScrollToBottom = true;
	}

	void    Draw(const char* title, bool* p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close"))
				*p_open = false;
			ImGui::EndPopup();
		}

		ImGui::TextWrapped("Enter 'CLEAR' to clear, HISTORY for history, Exit for exit, or lua code.");

		// TODO: display items starting from the bottom

		if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
		if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PopStyleVar();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings); // Leave room for 1 separator + 1 InputText
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
		// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
		// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
		//     ImGuiListClipper clipper(Items.Size);
		//     while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
		// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		for (int i = 0; i < Items.Size; i++)
		{
			const char* item = Items[i];
			ImVec4 col = col_default_text;
			if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();
		if (ScrollToBottom)
			ImGui::SetScrollHere();
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			char* input_end = InputBuf + strlen(InputBuf);
			while (input_end > InputBuf && input_end[-1] == ' ') { input_end--; } *input_end = 0;
			if (InputBuf[0])
				ExecCommand(InputBuf, p_open);
			strcpy(InputBuf, "");
			reclaim_focus = true;
		}

		// Demonstrate keeping focus on the input box
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}

	void    ExecCommand(const char* command_line, bool* p_open)
	{
		AddLog("# %s\n", command_line);

		// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
		HistoryPos = -1;
		for (int i = History.Size - 1; i >= 0; i--)
			if (Stricmp(History[i], command_line) == 0)
			{
				free(History[i]);
				History.erase(History.begin() + i);
				break;
			}
		History.push_back(Strdup(command_line));

		/* need to figure out a way to return function results */
		std::string result = luaState->do_string(command_line);

		if (Stricmp(command_line, "CLEAR") == 0)
		{
			ClearLog();
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = History.Size - 10;
			for (int i = first > 0 ? first : 0; i < History.Size; i++)
				AddLog("%3d: %s\n", i, History[i]);
		}
		else if (Stricmp(command_line, "EXIT") == 0)
		{
			*p_open = false;
		}
		else if (result.empty())
		{
			AddLog("You forgot the 'return' statement.");
		}
		else
		{
			AddLog(result.c_str());
		}
	}
};

namespace sl
{
	DebugInterface::DebugInterface(ALLEGRO_DISPLAY* display, bool isDisabled)
	:m_disabled(isDisabled)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplA5_Init(display);

		ImGui::StyleColorsDark();

		m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		m_editor.SetPalette(TextEditor::GetDarkPalette());
	}

	DebugInterface::~DebugInterface()
	{
		ImGui_ImplA5_Shutdown();
		ImGui::DestroyContext();
	}

	void DebugInterface::event(ALLEGRO_EVENT* event)
	{
		if (!m_disabled)
		{
			ImGui_ImplA5_ProcessEvent(event);
		}
	}

	void DebugInterface::newFrame()
	{
		if (!m_disabled)
		{
			ImGui_ImplA5_NewFrame();
		}
	}

	void DebugInterface::render()
	{
		if (!m_disabled)
		{
			ImGui::Render();
			ImGui_ImplA5_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void DebugInterface::displayMenu(bool* restart)
	{
		static bool s_showLuaConsole = false;
		static bool s_showScriptEditor = false;
		static std::string s_buff = "";

		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Restart Game"))
				{
					*restart = true;
					Locator::window->close();
				}

				if (ImGui::MenuItem("Quit Game"))
				{
					Locator::window->close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Script Editor"))
				{
					s_showScriptEditor = true;
				}

				if (ImGui::MenuItem("Show Console"))
				{
					s_showLuaConsole = true;
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		if (ImGui::InputText("Create Entity from Script", s_buff, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!s_buff.empty())
			{
				Locator::world->createEntity(s_buff);
			}
		}

		if (s_showScriptEditor)
		{
			static int s_index = 0;
			static bool s_showFilesToLoad = false;
			static std::string s_currentScript = "";
			static std::vector<std::string> s_files;

			ImGui::Begin("Script Editor", &s_showScriptEditor, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
			ImGui::SetWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver); //800, 600

			auto cpos = m_editor.GetCursorPosition();
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						s_showFilesToLoad = true;
					}

					if (ImGui::MenuItem("Save"))
					{
						Locator::virtualFS->writeToFile(s_currentScript, m_editor.GetText().c_str());
					}

					if (ImGui::MenuItem("Close"))
					{
						s_showScriptEditor = false;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = m_editor.IsReadOnly();

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

				ImGui::EndMenuBar();
			}

			if (s_showFilesToLoad)
			{
				s_index = 0;
				s_files.clear();
				s_currentScript = "";

				char** efl = PHYSFS_enumerateFiles("scripts");
				if (!efl)
				{
					LOG_S(WARNING) << "Could not load scripts!";
				}
				else
				{
					for (char** i = efl; *i != NULL; i++)
					{
						s_files.emplace_back("scripts/" + std::string(*i));
					}
				}

				if (efl != NULL)
				{
					PHYSFS_freeList(efl);
				}

				ImGui::Begin("Select Script", &s_showFilesToLoad, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				if (ImGui::ListBox("", &s_index, s_files))
				{
					s_currentScript = s_files[s_index];
					std::string loadedText = Locator::virtualFS->openAsString(s_currentScript);
					
					if (loadedText.empty())
					{
						LOG_S(WARNING) << "Script file was empty! Or Loading failed. Check for other errors in log.";
					}
					else
					{
						m_editor.SetText(loadedText);
					}

					s_showFilesToLoad = false;
				}

				ImGui::End();
			}

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
			static ExampleAppConsole console(&Locator::world->m_lua);
			console.Draw("Lua Console", &s_showLuaConsole);
		}

		ImGui::End();
	}
}

#endif