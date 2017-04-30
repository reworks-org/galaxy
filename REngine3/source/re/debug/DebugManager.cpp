//
//  DebugManager.cpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui.h"
#include "re/debug/imgui/imgui-SFML.h"

#include "re/graphics/Window.hpp"
#include "re/services/ServiceLocator.hpp"

#include "DebugManager.hpp"

namespace ImGui
{
namespace SFML
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };
    
    inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ImGui::Combo(label, currIndex, vector_getter,
                            static_cast<void*>(&values), values.size());
    }
    
    inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ImGui::ListBox(label, currIndex, vector_getter,
                              static_cast<void*>(&values), values.size());
    }
}
}

namespace re
{
	DebugManager::DebugManager()
	{
		#ifndef NDEBUG
			enable();
			m_aliveEntityNames.clear();
			m_deadEntityNames.clear();
		#else
			disable();
		#endif
	}

	DebugManager::~DebugManager()
	{
        if (m_enabled == false && m_init == true)
        {
            ImGui::SFML::Shutdown();
        }
	}

	void DebugManager::init(sf::RenderTarget& target, sf::Texture* fontTexture)
	{
		if (m_enabled == true)
		{
			ImGui::SFML::Init(target, fontTexture);
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
            window.resetGLStates();
			ImGui::Render();
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
		m_enabled = true;
	}

	void DebugManager::updateEntityNames()
	{
		if (m_enabled == true)
		{
			m_aliveEntityNames.clear();
			m_deadEntityNames.clear();

			for (auto& v : m_world->getAliveEntitys())
			{
				m_aliveEntityNames.push_back(v.second.m_name);
			}

			for (auto& v : m_world->getDeadEntitys())
			{
				m_deadEntityNames.push_back(v.second.m_name);
			}
		}
	}

	void DebugManager::useMenu()
	{
		if (m_enabled == true)
        {
			updateEntityNames();

            ImGui::Begin("DebugMenu", (bool*)false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
            
            static int indexAlive = 0;
			static int indexDead = 0;

			static bool isInAliveEntitys = true;

			ImGui::Checkbox("Access alive or dead entitys?", &isInAliveEntitys);

			if (isInAliveEntitys)
			{
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Text("Entity manager (alive): ");

				ImGui::SFML::Combo("Entity Selector", &indexAlive, m_aliveEntityNames);

				if (m_aliveEntityNames.empty() == false)
				{
					Entity* curEntity = &(m_world->getEntity(m_aliveEntityNames[indexAlive]));

					static bool killEntity = false;
					ImGui::Checkbox("Kill Entity?", &killEntity);

					if (killEntity == true)
					{
						m_world->killEntity(m_aliveEntityNames[indexAlive]);
						killEntity = false;
						indexAlive = 0;
					}
				}
			}
			else
			{
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Text("Entity manager (dead): ");

				ImGui::SFML::Combo("Entity Selector", &indexDead, m_deadEntityNames);

				if (m_deadEntityNames.empty() == false)
				{
					Entity* curEntity = &(m_world->getDeadEntity(m_deadEntityNames[indexDead]));

					static bool restoreEntity = false;
					ImGui::Checkbox("Restore Entity?", &restoreEntity);

					if (restoreEntity == true)
					{
						m_world->restoreEntity(m_deadEntityNames[indexDead]);
						restoreEntity = false;
						indexDead = 0;
					}
				}
			}
			
		    ImGui::End();
        }
	}
}
