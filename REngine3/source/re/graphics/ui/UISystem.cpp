//
//  UISystem.cpp
//  REngine3
//
//  Created by reworks on 29/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "UISystem.hpp"

namespace re
{
	UISystem::~UISystem()
	{
		// Clean up map.
		m_panels.clear();
	}

	void UISystem::AddPanel(const std::string& key, std::shared_ptr<UIPanel> panel)
	{
		// Add a panel to the map.
		m_panels.emplace(key, panel);
	}

	std::shared_ptr<UIPanel> UISystem::GetPanel(const std::string& key)
	{
		// Get a panel from the map.
		return m_panels[key];
	}

	void UISystem::Event()
	{
		// Handle events for all panels and thereby components.
		for (auto& it : m_panels) {
			it.second->Event();
		}
	}

	void UISystem::Update()
	{
		// Update all panels and thereby components.
		for (auto& it : m_panels) {
			it.second->Update();
		}
	}

	void UISystem::Render(re::Window* window)
	{
		// Render all panels and thereby components.
		for (auto& it : m_panels) {
			it.second->Render(window);
		}
	}
}