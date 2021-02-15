///
/// LuaConsole.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>
#include <sol/sol.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>
#include <galaxy/fs/FileSystem.hpp>

#include "LuaConsole.hpp"

// Thanks to: https://gist.github.com/5at/3671566
namespace
{
	std::vector<std::string> strs;

	int l_my_print(lua_State* L)
	{
		int nargs = lua_gettop(L);
		for (int i = 1; i <= nargs; ++i)
		{
			strs.emplace_back(lua_tostring(L, i));
		}

		return 0;
	}

	const struct luaL_Reg printlib[] = {
	    {"print", l_my_print},
	    {NULL, NULL}};
} // namespace

namespace sc
{
	namespace panel
	{
		LuaConsole::LuaConsole()
		{
			lua_getglobal(SL_HANDLE.lua()->lua_state(), "_G");
			luaL_setfuncs(SL_HANDLE.lua()->lua_state(), printlib, 0);
			lua_pop(SL_HANDLE.lua()->lua_state(), 1);
		}

		void LuaConsole::render()
		{
			ImGui::Begin("Lua Console", (bool*)true, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Run Script"))
				{
					auto file = SL_HANDLE.vfs()->show_open_dialog("*.lua");
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

					if (!strs.empty())
					{
						for (const auto& str : strs)
						{
							m_history.push_back(str);
						}

						strs.clear();
					}

					m_buff.clear();
					ImGui::SetKeyboardFocusHere(-1);
				}

				if (ImGui::MenuItem("Run GC"))
				{
					SL_HANDLE.lua()->collect_garbage();
				}

				if (ImGui::MenuItem("Clear"))
				{
					m_history.clear();
				}

				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				if (ImGui::InputText("", &m_buff, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
				{
					m_history.push_back(fmt::format("[INPUT]:  {0}.", m_buff));

					auto res  = SL_HANDLE.lua()->script(m_buff);
					auto type = res.get_type();
					std::string out;
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
	} // namespace panel
} // namespace sc