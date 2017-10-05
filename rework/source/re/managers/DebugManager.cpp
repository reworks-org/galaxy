//
//  DebugManager.cpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <sstream>
#include <fstream>

#include <allegro5/events.h>

#include "re/libs/imgui/imgui.h"
#include "re/libs/imgui/imgui_impl_a5.h"

#include "re/services/VFS.hpp"
#include "re/graphics/Window.hpp"
#include "re/managers/StateManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/SpriteComponent.hpp"

#include "DebugManager.hpp"

namespace re
{
	// https://stackoverflow.com/a/217605
	void trimFromEnd(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	DebugManager::DebugManager(ALLEGRO_DISPLAY* display)
	:m_reloadState(nullptr), m_enabled(false), m_init(false)
	{
		m_world = Locator::get<World>();
		ImGui_ImplA5_Init(display);
	}

	DebugManager::~DebugManager()
	{
		ImGui_ImplA5_Shutdown();
	}

	void DebugManager::event(ALLEGRO_EVENT* event)
	{
		ImGui_ImplA5_ProcessEvent(event);
	}

	void DebugManager::update()
	{
		ImGui_ImplA5_NewFrame();
	}

	void DebugManager::render()
	{
		ImGui::Render();
	}

	void DebugManager::displayMenu()
	{
		static int index = 0;
		static int indexComponent = 0;
		static bool showScriptEditor = false;

		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Reload State"))
		{
			showScriptEditor = false;
			m_reloadFunc();
			Locator::get<StateManager>()->reloadState(m_reloadState);
		}

		ImGui::Spacing();

		ImGui::Text("Entity Manager");
		ImGui::Spacing();

		if (m_world->m_loadedEntityScripts.empty() == false)
		{
			ImGui::SFML::Combo("Entity Selector", &index, m_world->m_loadedEntityScripts);

			size_t size = m_world->m_loadedEntityScripts.size();
			if ((size_t)index >= size)
			{
				index = (size - 1);
			}

			std::string curEntityScriptName = m_world->m_loadedEntityScripts[index];

			std::ifstream ifs(Locator::get<VFS>()->retrieve(curEntityScriptName));
			std::string curEntityScriptData((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			ifs.close();

			m_lua.script(curEntityScriptData);
			sol::table entityTable = m_lua.get<sol::table>("entity");
			Entity* curEntity = &(m_world->getEntity(entityTable.get<std::string>("name")));

			std::map<std::string, sol::table> m_keyValuePair;
			std::vector<std::string> componentNames;
			entityTable.for_each([&](std::pair<sol::object, sol::object> pair) {
				m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			// Remove stuff that isn't components
			m_keyValuePair.erase("name");
			m_keyValuePair.erase("systems");
			m_keyValuePair.erase("isDead");

			for (auto& it : m_keyValuePair)
			{
				componentNames.push_back(it.first);
			}

			ImGui::Spacing();

			if (ImGui::Button("Toggle Script Editor"))
			{
				if (showScriptEditor == true)
				{
					showScriptEditor = false;
				}
				else
				{
					showScriptEditor = true;
				}
			}

			if (showScriptEditor)
			{
				static std::vector<char> scriptData(curEntityScriptData.begin(), curEntityScriptData.end());
				static bool doneOnce = false;
				static std::string first = curEntityScriptName;

				if (!doneOnce)
				{
					// this should be large enough...i hope
					scriptData.resize(2000);
					doneOnce = true;
				}

				if (first != m_world->m_loadedEntityScripts[index])
				{
					first = m_world->m_loadedEntityScripts[index];

					scriptData.clear();
					scriptData = std::vector<char>(curEntityScriptData.begin(), curEntityScriptData.end());
					scriptData.resize(2000);
				}

				ImGui::SetNextWindowSize(ImVec2(420, 500), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowPosCenter();

				std::string name = "Script Editor: " + curEntityScriptName;
				ImGui::Begin(name.c_str(), (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::InputTextMultiline("", scriptData.data(), scriptData.size(), ImVec2(420, 500), ImGuiInputTextFlags_EnterReturnsTrue);

				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::Button("Save Changes"))
				{
					for (auto& v : scriptData)
					{
						if (!v)
						{
							v = ' ';
						}
					}

					std::string path = Locator::get<VFS>()->getBasePath() + curEntityScriptName;

					std::ofstream out;
					out.open(path, std::ofstream::out);

					std::string dataOut(scriptData.begin(), scriptData.end());
					trimFromEnd(dataOut);
					dataOut.erase(std::remove(dataOut.begin(), dataOut.end(), '\r'), dataOut.end());

					dataOut.shrink_to_fit();
					out << dataOut << std::endl;
					out.close();

					scriptData.clear();
					scriptData = std::vector<char>(dataOut.begin(), dataOut.end());
					scriptData.resize(2000);
				}

				ImGui::End();
			}

			ImGui::Spacing();
			ImGui::Text("%s", std::string("Name: " + curEntity->m_name).c_str());

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

				ImGui::Separator();
				ImGui::Spacing();

				curEntity->useComponentDebugFunction(curComponent, curTable, entityTable.get<std::string>("name"));
			}
		}

		ImGui::End();
	}

	void DebugManager::specifyReloadState(std::shared_ptr<State> s, std::function<void(void)> func)
	{
		m_reloadState = s;
		m_reloadFunc = func;
	}
}
