//
//  DebugManager.cpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui.h"
#include "re/debug/imgui/imgui-SFML.h"
#include "re/services/ServiceLocator.hpp"

#include "re/graphics/Window.hpp"

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
			m_entityNames.clear();
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
			m_entityNames.clear();

			for (auto& v : Locator::get<World>()->getAliveEntitys())
			{
				m_entityNames.push_back(v.second.m_name);
			}

			for (auto& v : Locator::get<World>()->getDeadEntitys())
			{
				m_entityNames.push_back(v.second.m_name);
			}
		}
	}

	void DebugManager::useMenu()
	{
		if (m_enabled == true)
        {
            ImGui::Begin("DebugMenu");
            
            //ImGui::ShowTestWindow();

            static int index = 0;
                       
			ImGui::SFML::Combo("EntitySelector", &index, m_entityNames);
			
		    ImGui::End();
        }
	}
}
