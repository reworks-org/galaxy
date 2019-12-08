// ImGui console implementation
// Based off of imgui_demo console implementation.

#ifndef imgui_console_h
#define imgui_console_h

#include <filesystem>

#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/libs/imgui/imgui.h"
#include "galaxy/core/ServiceLocator.hpp"

namespace ImGui
{
	///
	/// Based off of example in imgui demo, under same license.
	/// Represents a console.
	/// Been modified by reworks for use in galaxy.
	///
	struct Console
	{
		char                  InputBuf[256];
		ImVector<char*>       Items;
		bool                  ScrollToBottom;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.

		Console()
		{
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;
		}

		~Console()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
		}

		 // Portable helpers
	    static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	    static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	    static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
	    static void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

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
	        buf[IM_ARRAYSIZE(buf)-1] = 0;
	        va_end(args);
	        Items.push_back(Strdup(buf));
	        ScrollToBottom = true;
	    }

		void    Draw(const char* title, bool* p_open)
	    {
	        ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
	        if (!ImGui::Begin(title, p_open))
	        {
	            ImGui::End();
	            return;
	        }

	        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
	        // Here we create a context menu only available from the title bar.
	        if (ImGui::BeginPopupContextItem())
	        {
	            if (ImGui::MenuItem("Close Console"))
	                *p_open = false;
	            ImGui::EndPopup();
	        }

	        ImGui::TextWrapped("Enter 'CLEAR' to clear, HISTORY for history, EXIT for exit, or lua code. Prefix lua code with 'return' to get an output.");
			ImGui::TextWrapped("Enter a file with a .lua extension to attempt to execute that file.");

	        // TODO: display items starting from the bottom

	        if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
	        if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
	        if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
	        bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
	        if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
	        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

	        ImGui::Separator();

	        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
	        static ImGuiTextFilter filter;
	        filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	        ImGui::PopStyleVar();
	        ImGui::Separator();

	        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
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
	        // However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
	        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
	        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
	        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
	        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
	        if (copy_to_clipboard)
	            ImGui::LogToClipboard();
	        ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	        for (int i = 0; i < Items.Size; i++)
	        {
	            const char* item = Items[i];
	            if (!filter.PassFilter(item))
	                continue;
	            ImVec4 col = col_default_text;
	            if (strstr(item, "[error]")) col = ImColor(1.0f,0.4f,0.4f,1.0f);
	            else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
	            ImGui::PushStyleColor(ImGuiCol_Text, col);
	            ImGui::TextUnformatted(item);
	            ImGui::PopStyleColor();
	        }
	        if (copy_to_clipboard)
	            ImGui::LogFinish();
	        if (ScrollToBottom)
	            ImGui::SetScrollHere(1.0f);
	        ScrollToBottom = false;
	        ImGui::PopStyleVar();
	        ImGui::EndChild();
	        ImGui::Separator();

	        // Command-line
	        bool reclaim_focus = false;
	        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	        {
	            char* s = InputBuf;
	            Strtrim(s);
	            if (s[0])
	                ExecCommand(s, p_open);
	            strcpy(s, "");
	            reclaim_focus = true;
	        }

	        // Auto-focus on window apparition
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
	        for (int i = History.Size-1; i >= 0; i--)
	            if (Stricmp(History[i], command_line) == 0)
	            {
	                free(History[i]);
	                History.erase(History.begin() + i);
	                break;
	            }
	        History.push_back(Strdup(command_line));

	        // Process command
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
			else if (std::filesystem::path(command_line).extension() == ".lua")
			{
				if (galaxy::Locator::virtualFS->has(command_line))
				{
					std::string result = galaxy::Locator::lua->do_string(galaxy::Locator::virtualFS->openAsString(command_line));
					AddLog(result.c_str());
				}
				else
				{
					AddLog("ERROR: Script not found!");
				}
				
			}
			else
			{
				std::string result = galaxy::Locator::lua->do_string(command_line);
				AddLog(result.c_str());
			}
		}
	};
}

#endif