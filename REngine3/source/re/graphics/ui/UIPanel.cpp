//
//  UIPanel.cpp
//  REngine3
//
//  Created by reworks on 29/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <typeinfo>

#include "re/deps/sol2/sol.hpp"

#include "re/graphics/Window.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "UIPanel.hpp"

namespace re
{
	UIPanel::UIPanel(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::Retrieve<VFS>()->ToString(script));

		sol::table panel = lua.get<sol::table>("UIPanel");
		sol::table texCoords = panel.get<sol::table>("texCoords");

		m_xpos = panel.get<int>("x");
		m_ypos = panel.get<int>("y");

		m_spriteArea.left = texCoords.get<int>("x");
		m_spriteArea.top = texCoords.get<int>("y");
		m_spriteArea.width = texCoords.get<int>("w");
		m_spriteArea.height = texCoords.get<int>("h");

		m_textureStream.open(panel.get<std::string>("texture"));
		m_texture.loadFromStream(m_textureStream);

		m_sprite.setTexture(m_texture);
		m_sprite.setTextureRect(m_spriteArea);

		m_sprite.setPosition((float)m_xpos, (float)m_ypos);
	}

	UIPanel::~UIPanel()
	{
		// Clean up map.
		m_uicomponents.clear();

		// Clean up deleted components.
		m_deletedComponents.clear();
	}

	void UIPanel::Add(const std::string& id, std::shared_ptr<UIComponent> ui)
	{
		// Add panel to map.
		m_uicomponents.emplace(id, ui);
	}

	void UIPanel::SetComponentOffsets()
	{
		// Set offsets of all components.
		for (auto& it : m_uicomponents)
		{
			it.second->SetOffset(m_xpos, m_ypos);
		}
	}

	void UIPanel::SetVisibility(bool isVisible)
	{
		m_isVisible = isVisible;
	}

	void UIPanel::Event()
	{
		// Handle events for all components.
		for (auto& it : m_uicomponents) 
		{
			it.second->Event();
		}
	}

	void UIPanel::Update()
	{
		// Update all components.
		for (auto& it : m_uicomponents)
		{
			it.second->Update();
		}
	}

	void UIPanel::Render(re::Window* window)
	{
		// Render panel and thereby the components if its visible.
		m_isVisible = true;
		if (m_isVisible)
		{
			window->draw(m_sprite);

			for (auto& it : m_uicomponents)
			{
				window->draw(*it.second.get(), &m_texture);
			}

		}
	}
	
	void UIPanel::DeleteComponent(const std::string& id)
	{
		// Find the component with key and save it as iterator.
		auto it = m_uicomponents.find(id);
		
		// Move to deleted components map for storage, instead of permanently deleting it.
		m_deletedComponents.emplace(id, it->second);

		// Erase from original.
		m_uicomponents.erase(id);
	}

	void UIPanel::RestoreComponent(const std::string& id)
	{
		// Find the component with key and save it as iterator.
		auto it = m_deletedComponents.find(id);

		// Restore to original map.
		m_uicomponents.emplace(id, it->second);

		// Erase from deleted map.
		m_deletedComponents.erase(id);
	}
}