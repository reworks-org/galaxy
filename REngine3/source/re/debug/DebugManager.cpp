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

namespace re
{
	DebugManager::DebugManager()
	{
		#ifndef NDEBUG
			enable();
		#else
			disable();
		#endif
	}

	DebugManager::~DebugManager()
	{
	}

	void DebugManager::init(sf::RenderTarget& target, sf::Texture* fontTexture)
	{
		if (m_enabled)
		{
			ImGui::SFML::Init(target, fontTexture);

			m_alive = &(Locator::get<World>()->getAliveEntitys());
			m_dead = &(Locator::get<World>()->getDeadEntitys());
		}
	}

	void DebugManager::event(const sf::Event& event)
	{
		if (m_enabled)
		{
			ImGui::SFML::ProcessEvent(event);
		}
	}

	void DebugManager::update(sf::RenderWindow& window, sf::Time dt)
	{
		if (m_enabled)
		{
			ImGui::SFML::Update(window, dt);
		}
	}

	void DebugManager::render()
	{
		if (m_enabled)
		{
			ImGui::Render();
		}
	}

	void DebugManager::cleanup()
	{
		if (m_enabled)
		{
			ImGui::SFML::Shutdown();
		}
	}

	void DebugManager::disable()
	{
		m_enabled = false;
	}

	bool DebugManager::isDisabled()
	{
		return m_enabled;
	}

	void DebugManager::enable()
	{
		m_enabled = true;
	}

	void DebugManager::useMenu()
	{
		ImGui::Begin("debugMenu");
		char windowTitle[255] = "Text here";
		// Window title text edit
		ImGui::InputText("Window title", windowTitle, 255);

		if (ImGui::Button("Update window title")) {
			// this code gets if user clicks on the button
			// yes, you could have written if(ImGui::InputText(...))
			// but I do this to show how buttons work :)
			Locator::get<Window>()->setTitle(windowTitle);
		}

		ImGui::End();
	}
}