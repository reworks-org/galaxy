///
/// LuaConsole.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <imgui/imgui_stdlib.h>
#include "galaxy/error/Log.hpp"
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "LuaConsole.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	LuaConsole::LuaConsole()
	    : m_buff {""}
	{
	}

	void LuaConsole::draw(bool* show)
	{
		ImGui::Begin("Lua Console", show, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Close"))
			{
				*show = false;
			}

			if (ImGui::Button("Clear"))
			{
				m_history.clear();
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			if (ImGui::InputText("", &m_buff, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
			{
				m_history.push_back(fmt::format("[INPUT]:  {0}.", m_buff));
				std::string code = {""};

				if (std::filesystem::path(m_buff).extension() == ".lua")
				{
					std::ifstream ifs;
					ifs.open(std::filesystem::path(m_buff).string(), std::ifstream::in);
					code = std::string((std::istreambuf_iterator<char>(ifs)),
							   std::istreambuf_iterator<char>());
				}
				else
				{
					code = m_buff;
				}

				try
				{
					const std::string res = SL_HANDLE.lua()->safe_script(code);
					m_history.push_back(fmt::format("[RESULT]: {0}.", res));
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, e.what());
					m_history.push_back(fmt::format("[ERROR]:  {0}.", e.what()));
				}

				m_buff.clear();
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