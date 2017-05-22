//
//  DebugManager.cpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <sstream>
#include <fstream>

#include "re/debug/imgui/imgui.h"
#include "re/debug/imgui/imgui-SFML.h"

#include "re/graphics/Window.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/services/vfs/VFS.hpp"

#include "DebugManager.hpp"

namespace ImGui
{
namespace SFML
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };
    
    inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ImGui::Combo(label, currIndex, vector_getter,
                            static_cast<void*>(&values), values.size());
    }
    
    inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ImGui::ListBox(label, currIndex, vector_getter,
                              static_cast<void*>(&values), values.size());
    }
}

// Modified version of the imgui app console!
// Taken from ImGui sources.
struct AppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    bool                  ScrollToBottom;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImVector<const char*> Commands;
    sol::state* m_lua;

    AppConsole(sol::state* lua)
    {
    	m_lua = lua;
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        Commands.push_back("help");
        Commands.push_back("history");
        Commands.push_back("clear");
        AddLog("Welcome to ImGui!");
    }
    ~AppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
    static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
    static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
        ScrollToBottom = true;
    }

    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
    {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
        ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiSetCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
        if (ImGui::SmallButton("Add Dummy Error")) AddLog("[error] something went wrong"); ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) ClearLog(); ImGui::SameLine();
        if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
        static ImGuiTextFilter filter;
        filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::PopStyleVar();
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
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
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!filter.PassFilter(item))
                continue;
            ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
            if (strstr(item, "[error]")) col = ImColor(1.0f,0.4f,0.4f,1.0f);
            else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::TextUnformatted(item);
            ImGui::PopStyleColor();
        }
        if (ScrollToBottom)
            ImGui::SetScrollHere();
        ScrollToBottom = false;
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
        {
            char* input_end = InputBuf+strlen(InputBuf);
            while (input_end > InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
            if (InputBuf[0])
                ExecCommand(InputBuf);
            strcpy(InputBuf, "");
        }

        // Demonstrate keeping auto focus on the input box
        if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size-1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "clear") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "help") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "history") == 0)
        {
            for (int i = History.Size >= 10 ? History.Size - 10 : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            auto result = m_lua->script(std::string(command_line));
            AddLog(std::to_string<auto>(result));
        }
    }

    static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    {
        AppConsole* console = (AppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiTextEditCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // No match
                    AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                    data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    AddLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog("- %s\n", candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (HistoryPos >= 0) ? History[HistoryPos] : "");
                    data->BufDirty = true;
                }
            }
        }
        return 0;
    }
};
}

namespace re
{
	DebugManager::DebugManager()
	{
		#ifndef NDEBUG
			enable();
		#else
			disable();
		#endif
	}

	DebugManager::~DebugManager()
	{
        if (m_enabled == false && m_init == true)
        {
            ImGui::SFML::Shutdown();
        }
	}

	void DebugManager::init(sf::RenderTarget& target, sf::Texture* fontTexture)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Init(target, fontTexture);
            m_init = true;

			m_world = Locator::get<World>();
		}
	}

	void DebugManager::event(const sf::Event& event)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::ProcessEvent(event);
		}
	}

	void DebugManager::update(sf::RenderWindow& window, sf::Time dt)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Update(window, dt);
		}
	}

	void DebugManager::render(sf::RenderWindow& window)
	{
		if (m_enabled == true)
		{
            window.resetGLStates();
			ImGui::Render();
		}
	}

	void DebugManager::cleanup()
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Shutdown();
			m_init = false;
		}
	}

	void DebugManager::disable()
	{
		m_enabled = false;
	}

	bool DebugManager::isEnabled()
	{
		return m_enabled;
	}

	void DebugManager::enable()
	{
		m_enabled = true;
	}

	void DebugManager::useMenu()
	{
		if (m_enabled == true)
        {
            static int index = 0;
            static int indexComponent = 0;
            
            static bool showConsole = false;
            static bool showScriptEditor = false;
            static ImGui::AppConsole luaConsole(&m_lua);
            
            ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::Spacing();
            
            // refer to demo guide to see how to keep open
            showConsole = ImGui::Button("Show lua console");
            if (showConsole)
            {
                luaConsole.Draw("Lua Console", true);
            }
            
            ImGui::Spacing();

            ImGui::Text("Entity Manager");
            ImGui::Separator();
            
            if (m_world->m_loadedEntityScripts.empty() == false)
            {
                ImGui::SFML::Combo("Entity Selector", &index, m_world->m_loadedEntityScripts);
                
				size_t size = m_world->m_loadedEntityScripts.size();
				if ((size_t)index >= size)
				{
					index = (size - 1);
				}

				std::string curEntityScriptName = m_world->m_loadedEntityScripts[index];
                m_lua.script(Locator::get<VFS>()->toString(curEntityScriptName));
                sol::table entityTable = m_lua.get<sol::table>("entity");
                Entity* curEntity = &(m_world->getEntity(entityTable.get<std::string>("name")));
                
				std::map<std::string, sol::table> m_keyValuePair;
				std::vector<std::string> componentNames;
				entityTable.for_each([&](std::pair<sol::object, sol::object> pair) {
					m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
				});

				m_keyValuePair.erase("name");

				for (auto& it : m_keyValuePair)
				{
					componentNames.push_back(it.first);
				}

                ImGui::Spacing();
                showScriptEditor = ImGui::Button("Open Script Editor");
                if (showScriptEditor)
                {
                    // open script file
                    // use imgui multiline text editor to edit
                    // write out script file
                    // use vfs, so i will need to modify it to support writing out
                    // Physfs_write() or something like that.
                }
                
				ImGui::Spacing();
                ImGui::Text(std::string("Name: " + curEntity->m_name).c_str());

				ImGui::Spacing();
				std::string stateButtonText = "Kill Entity";
				if (curEntity->isDead())
				{
					stateButtonText = "Revive Entity";
				}

				if (ImGui::Button(stateButtonText.c_str()))
				{
					if (stateButtonText == "Kill Entity")
					{
						m_world->killEntity(curEntity->m_name);
					}
					else
					{
						m_world->restoreEntity(curEntity->m_name);
					}
				}
				else
				{
					ImGui::Spacing();
					ImGui::SFML::Combo("Component Selector", &indexComponent, componentNames);
                    
					if ((size_t)indexComponent >= componentNames.size())
					{
						indexComponent = (componentNames.size() - 1);
					}

                    std::string curComponent = componentNames[indexComponent];
                    sol::table curTable = entityTable.get<sol::table>(curComponent);

                    curEntity->useComponentDebugFunction(curComponent, curTable);
				}
            }
           
		    ImGui::End();
        }
	}
}
