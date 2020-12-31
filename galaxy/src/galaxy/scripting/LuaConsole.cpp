///
/// LuaConsole.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include <imgui/imgui_stdlib.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "LuaConsole.hpp"

namespace galaxy
{
	namespace lua
	{
		Console::Console()
		    : m_buff {""}
		{
		}

		void Console::draw(bool* show)
		{
			ImGui::Begin("Lua Console", show, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
			ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Run Script"))
				{
					auto file = fs::open_file_dialog("*.lua");
					std::ifstream ifs;
					ifs.open(std::filesystem::path(file).string(), std::ifstream::in);
					m_buff = std::string((std::istreambuf_iterator<char>(ifs)),
							     std::istreambuf_iterator<char>());

					m_history.push_back("[SCRIPT]: ");
					m_history.push_back(m_buff);

					auto res        = SL_HANDLE.lua()->script(m_buff);
					auto type       = res.get_type();
					std::string out = "";
					if (type == sol::type::string)
					{
						out = res.get<std::string>();
					}
					else if (type == sol::type::number)
					{
						out = std::to_string(res.get<float>());
					}
					else if (type == sol::type::boolean)
					{
						if (res.get<bool>())
						{
							out = "true";
						}
						else
						{
							out = "false";
						}
					}

					if (!out.empty())
					{
						m_history.push_back(fmt::format("[RESULT]: {0}.", out));
					}

					m_buff.clear();
					ImGui::SetKeyboardFocusHere(-1);
				}
				if (ImGui::MenuItem("Clear"))
				{
					m_history.clear();
				}

				if (ImGui::MenuItem("Close"))
				{
					*show = false;
				}

				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				if (ImGui::InputText("", &m_buff, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
				{
					m_history.push_back(fmt::format("[INPUT]:  {0}.", m_buff));

					auto res        = SL_HANDLE.lua()->script(m_buff);
					auto type       = res.get_type();
					std::string out = "";
					if (type == sol::type::string)
					{
						out = res.get<std::string>();
					}
					else if (type == sol::type::number)
					{
						out = std::to_string(res.get<float>());
					}
					else if (type == sol::type::boolean)
					{
						if (res.get<bool>())
						{
							out = "true";
						}
						else
						{
							out = "false";
						}
					}

					if (!out.empty())
					{
						m_history.push_back(fmt::format("[RESULT]: {0}.", out));
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
	} // namespace lua
} // namespace galaxy