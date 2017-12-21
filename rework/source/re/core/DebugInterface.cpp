///
///  DebugInterface.cpp
///  rework
///
///  Created by reworks on 20/04/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <physfs.h>
#include <allegro5/events.h>
#include <allegro5/display.h>

#include "sol2/sol.hpp"
#include "re/fs/VFS.hpp"
#include "re/core/World.hpp"
#include "imgui/imgui_impl_a5.h"
#include "re/core/StateManager.hpp"

#include "DebugInterface.hpp"

namespace re
{
	DebugInterface::DebugInterface(ALLEGRO_DISPLAY* display)
	:m_reloadState(nullptr), m_disabled(false), m_doneOnce(false)
	{
		ImGui_ImplA5_Init(display);
	}

	DebugInterface::~DebugInterface()
	{
		ImGui_ImplA5_Shutdown();
	}

	void DebugInterface::disable(bool isDisabled)
	{
		m_disabled = isDisabled;
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
		}
	}

	void DebugInterface::displayMenu()
	{
		static std::vector<std::string> m_scripts;
		static int scriptIndex = 0;

		if (!m_doneOnce)
		{
			char** scriptArray = PHYSFS_enumerateFiles("scripts");
			for (char** i = efl; *i != NULL; ++i)
			{
				m_scripts.emplace_back(*i);
			}
			PHYSFS_freeList(scriptArray);

			m_doneOnce = true;
		}

		if (!m_disabled)
		{
			ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

			if (ImGui::Button("Reload to State"))
			{
				m_reloadFunc();
				StateManager::get()->reloadState(m_reloadState);
			}
			ImGui::Spacing();

			ImGui::Text("Entity Editor");
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::stl::Combo("Select script:", &scriptIndex, m_scripts))
			{
				m_lua.script(VFS::get()->openAsString(m_scripts[scriptIndex]));
			}
			
			/*
			https://github.com/Csabix/imgui/tree/master/auto
			https://github.com/pkulchenko/serpent
			https://github.com/BalazsJako/ImGuiColorTextEdit
			https://github.com/ocornut/imgui/issues/300
			https://github.com/Flix01/imgui/tree/2015-10-Addons/addons
			*/
			
			
			
			
			
			
			
			
			ImGui::stl::Combo("Entity Selector", &index, entityScripts);

			size_t size = entityScripts.size();
			if ((size_t)index >= size)
			{
				index = (size - 1);
			}

			std::string curEntityScriptName = entityScripts[index];
			std::string curEntityScriptData = m_world->m_entityScripts[entityScripts[index]];

			m_lua.script(curEntityScriptData);
			sol::table entityTable = m_lua.get<sol::table>("entity");
			entityx::Entity& e = m_world->m_entitys[entityTable.get<std::string>("name")];

			std::map<std::string, sol::table> kvp;
			std::vector<std::string> componentNames;
			entityTable.for_each([&](std::pair<sol::object, sol::object> pair) {
				kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			// Remove stuff that isn't components
			kvp.erase("name");
			kvp.erase("special");

			for (auto& it : kvp)
			{
				componentNames.push_back(it.first);
			}

			ImGui::Spacing();
			ImGui::stl::Combo("Component Selector", &indexComponent, componentNames);

			if ((size_t)indexComponent >= componentNames.size())
			{
				indexComponent = (componentNames.size() - 1);
			}

			std::string curComponent = componentNames[indexComponent];

			ImGui::Separator();
			ImGui::Spacing();

			m_world->m_componentDebug[curComponent](e);

			ImGui::End();
		}
	}

	void DebugInterface::specifyReloadState(std::shared_ptr<BaseState> s, std::function<void(void)> func)
	{
		m_reloadState = s;
		m_reloadFunc = func;
	}
}