///
/// LuaConsole.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <imgui_stdlib.h>
#include <sol/sol.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>

#include "LuaConsole.hpp"

using namespace galaxy;

// Thanks to: https://gist.github.com/5at/3671566
namespace
{
	meta::vector<std::string> strs;

	int l_my_print(lua_State* L)
	{
		int nargs = lua_gettop(L);
		for (int i = 1; i <= nargs; ++i)
		{
			strs.emplace_back(lua_tostring(L, i));
		}

		return 0;
	}

	const struct luaL_Reg printlib[] = {{"print", l_my_print}, {NULL, NULL}};
} // namespace

namespace sc
{
	namespace panel
	{
		LuaConsole::LuaConsole()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua_getglobal(lua.lua_state(), "_G");
			luaL_setfuncs(lua.lua_state(), printlib, 0);
			lua_pop(lua.lua_state(), 1);
		}

		LuaConsole::~LuaConsole()
		{
			m_buff.clear();
			m_history.clear();
		}

		void LuaConsole::render()
		{
			if (ImGui::Begin("Lua Terminal", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Open & Run Script"))
					{
						const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"}, "scripts/");
						if (!path.empty())
						{
							std::ifstream ifs;
							ifs.open(std::filesystem::path(path).string(), std::ifstream::in);
							m_buff = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

							m_history.push_back("[SCRIPT]: ");
							m_history.push_back(m_buff + '\n');

							auto res = core::ServiceLocator<sol::state>::ref().script(m_buff);

							const auto type = res.get_type();
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
								m_history.push_back(std::format("[RESULT]: {0}.", out));
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
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to open a file for Lua Console.");
						}
					}

					if (ImGui::MenuItem("Run GC"))
					{
						core::ServiceLocator<sol::state>::ref().collect_garbage();
					}

					if (ImGui::MenuItem("Clear"))
					{
						m_history.clear();
					}

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

					if (ImGui::InputText("",
							&m_buff,
							ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoUndoRedo))
					{
						m_history.push_back(std::format("[INPUT]:  {0}.", m_buff));

						try
						{
							auto res  = core::ServiceLocator<sol::state>::ref().script(m_buff);
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
								m_history.push_back(std::format("[RESULT]: {0}.", out));
							}
						}
						catch (const std::exception& e)
						{
							m_history.push_back(e.what());
						}

						m_buff.clear();
						ImGui::SetKeyboardFocusHere(-1);
					}

					ImGui::PopStyleColor(2);
					ImGui::EndMenuBar();
				}

				for (const auto& str : m_history)
				{
					ImGui::TextWrapped(str.c_str());
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc