//
//  DebugManager.cpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <sstream>
#include <fstream>

#include "re/debug/imgui/imgui-SFML.h"

#include "re/graphics/Window.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/systems/StateManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/SpriteComponent.hpp"

#include "DebugManager.hpp"

namespace re
{
	// https://stackoverflow.com/a/217605
	void trimFromEnd(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	DebugManager::DebugManager()
	{
	}

	DebugManager::~DebugManager()
	{
        if (m_enabled == false && m_init == true)
        {
            ImGui::SFML::Shutdown();
        }
	}

	void DebugManager::init(sf::RenderTarget& target, bool debugEnabled)
	{
		m_enabled = debugEnabled;

		if (m_enabled == true)
		{
			ImGui::SFML::Init(target, true);
            m_init = true;

			m_world = Locator::get<World>();
		}
	}

	void DebugManager::event(const sf::Event& event)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::ProcessEvent(event);
		}
	}

	void DebugManager::update(sf::RenderWindow& window, sf::Time dt)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Update(window, dt);
		}
	}

	void DebugManager::render(sf::RenderWindow& window)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Render(window);
		}
	}

	void DebugManager::cleanup()
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Shutdown();
			m_init = false;
		}
	}

	void DebugManager::disable()
	{
		m_enabled = false;
	}

	bool DebugManager::isEnabled()
	{
		return m_enabled;
	}

	void DebugManager::enable()
	{
		if (m_init == false)
		{
			ImGui::SFML::Init(target, true);
			m_init = true;

			m_world = Locator::get<World>();
		}

		m_enabled = true;
	}

	void DebugManager::useMenu()
	{
		if (m_enabled == true)
        {
            static int index = 0;
            static int indexComponent = 0;
            static bool showScriptEditor = false;
            
            ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
			
			if (ImGui::Button("Reload State"))
			{
				Locator::get<StateManager>()->reloadState();
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
                std::string curEntityScriptData = Locator::get<VFS>()->toString(curEntityScriptName);
                m_lua.script(curEntityScriptData);
                sol::table entityTable = m_lua.get<sol::table>("entity");
                Entity* curEntity = &(m_world->getEntity(entityTable.get<std::string>("name")));
                
				std::map<std::string, sol::table> m_keyValuePair;
				std::vector<std::string> componentNames;
				entityTable.for_each([&](std::pair<sol::object, sol::object> pair) {
					m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
				});

				m_keyValuePair.erase("name");

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

                    ImGui::SetNextWindowSize(ImVec2(420,500), ImGuiSetCond_FirstUseEver);
					ImGui::SetNextWindowPosCenter();

                    std::string name = "Script Editor: " + curEntityScriptName;
                    ImGui::Begin(name.c_str(), (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
                    ImGui::Separator();
                    ImGui::Spacing();
                    
					ImGui::InputTextMultiline("", scriptData.data(), scriptData.size(), ImVec2(420,500), ImGuiInputTextFlags_EnterReturnsTrue);
                    
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

						std::string path = Locator::get<VFS>()->getDir(curEntityScriptName) + curEntityScriptName;
						
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
                ImGui::Text(std::string("Name: " + curEntity->m_name).c_str());

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
	}

	/*void DebugManager::useMenu()
	{
		ImGui::ShowTestWindow();
	}*/
}
