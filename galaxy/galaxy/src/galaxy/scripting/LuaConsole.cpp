///
/// LuaConsole.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "LuaConsole.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	void LuaConsole::draw(bool* show)
	{
		ImGui::Begin("Lua Console", show, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Close"))
			{
				// MenuItem
				*show = false;
			}

			if (ImGui::Button("Clear"))
			{
				m_history.clear();
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			static std::string s_buff = {""};
			if (ImGui::InputText("", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
			{
				m_history.push_back("INPUT: " + s_buff);
				std::string code = {""};

				if (std::filesystem::path(s_buff).extension() == ".lua")
				{
					std::ifstream ifs;
					ifs.open(std::filesystem::path(s_buff).string(), std::ifstream::in);
					code = std::string((std::istreambuf_iterator<char>(ifs)),
							   std::istreambuf_iterator<char>());
				}
				else
				{
					code = s_buff;
				}

				std::string res = SL_HANDLE.lua()->safe_script(code);
				m_history.push_back("OUTPUT: " + res);

				s_buff.clear();
				ImGui::SetKeyboardFocusHere(-1);
			}

			ImGui::PopStyleColor(2);
			ImGui::EndMenuBar();
		}

		ImGui::Separator();
		ImGui::Spacing();

		for (const auto& str : m_history)
		{
			ImGui::TextWrapped(str.c_str());
		}

		ImGui::End();
	}
} // namespace galaxy