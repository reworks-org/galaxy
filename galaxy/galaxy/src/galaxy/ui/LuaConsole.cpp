///
/// LuaConsole.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <imgui.h>
#include <sol/sol.hpp>
#include <imgui_stdlib.h>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "LuaConsole.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	void LuaConsole::draw(bool* show) noexcept
	{
		ImGui::Begin("Lua Console", show, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

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

			static std::string s_buff = "";
			if (ImGui::InputText("", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
			{
				m_history.push_back(s_buff);
				std::string code = "";

				if (std::filesystem::path(s_buff).extension() == ".lua")
				{
					std::ifstream ifs(std::filesystem::path(s_buff).string());
					code = std::string((std::istreambuf_iterator<char>(ifs)),
						std::istreambuf_iterator<char>());
				}
				else
				{
					code = s_buff;
				}

				auto res = SL_HANDLE.lua()->safe_script(code);
				if (res.get_type() == sol::type::boolean)
				{
					bool val = res.get<bool>();
					if (val)
					{
						m_history.push_back("true");
					}
					else
					{
						m_history.push_back("false");
					}
				}
				else if (res.get_type() == sol::type::number)
				{
					double val = res.get<double>();
					m_history.push_back(std::to_string(val));
				}
				else if (res.get_type() == sol::type::string)
				{
					m_history.push_back(res.get<std::string>());
				}
				else
				{
					m_history.push_back("Success!");
				}

				s_buff.clear();
				ImGui::SetKeyboardFocusHere(-1);
			}

			ImGui::EndMenuBar();
		}

		ImGui::Separator();
		ImGui::Spacing();

		for (auto& str : m_history)
		{
			ImGui::TextWrapped(str.c_str());
		}

		ImGui::End();
	}
}