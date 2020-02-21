///
/// LuaConsole.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <any>
#include <filesystem>

#include <imgui.h>
#include <sol/sol.hpp>
#include <misc/cpp/imgui_stdlib.h>

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

				if (std::filesystem::path(s_buff).extension() == ".lua")
				{
					if (galaxy::ServiceLocator::get().fs()->has(s_buff))
					{
						auto code = galaxy::ServiceLocator::get().fs()->read(s_buff);
						auto res = galaxy::ServiceLocator::get().lua()->safe_script(code);
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
					}
					else
					{
						m_history.push_back("ERROR: Script not found!");
					}
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