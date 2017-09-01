//
//  SpriteComponent.cpp
//  REngine3
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "re/app/World.hpp"
#include "re/services/VFS.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/debug/imgui/imgui-sfml.h"
#include "re/services/ServiceLocator.hpp"
#include "re/utility/ResourceManager.hpp"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent()
		:m_group(0)
	{
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::init(sol::table& table)
	{
		m_group = table.get<sf::Uint32>("group");

		loadTexture(table);
	}

	void SpriteComponent::loadSingleTexture(const std::string& texture)
	{
		m_texture.loadFromFile(Locator::get<VFS>()->retrieve(texture));
		m_sprite.setTexture(m_texture);
	}

	void SpriteComponent::loadTexture(sol::table& table, const std::string& texture)
	{
		sol::object texRect = table["spriteRect"];

		if (texRect != sol::nil)
		{
			if (texture == "")
			{
				m_sprite.setTexture(Locator::get<ResourceManager<sf::Texture>>()->get(table.get<std::string>("altas_ref")));
			}
			else
			{
				m_sprite.setTexture(Locator::get<ResourceManager<sf::Texture>>()->get(table.get<std::string>(texture)));
			}

			sol::table rect = table.get<sol::table>("spriteRect");
			m_sprite.setTextureRect(sf::IntRect(rect.get<int>("tx"), rect.get<int>("ty"), rect.get<int>("tw"), rect.get<int>("th")));
		}
		else
		{
			if (texture == "")
			{
				m_texture.loadFromFile(Locator::get<VFS>()->retrieve(table.get<std::string>("texture")));
			}
			else
			{
				m_texture.loadFromFile(Locator::get<VFS>()->retrieve(texture));
			}

			m_sprite.setTexture(m_texture);
		}
	}

	void SpriteComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Text("%s", std::string("Group: " + std::to_string(m_group)).c_str());

		static std::string texture = table.get<std::string>("texture");
		static std::vector<char> buff(texture.begin(), texture.end());
		static bool doneOnce = false;
		static std::string originalEntityName = curEntityName;

		if (!doneOnce)
		{
			buff.resize(255);
			doneOnce = true;
		}

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			texture = table.get<std::string>("texture");
			buff.clear();
			buff = std::vector<char>(texture.begin(), texture.end());
			buff.resize(255);
		}

		if (ImGui::InputText("TexturePicker", buff.data(), buff.size(), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			for (auto& v : buff)
			{
				if (!v)
				{
					v = ' ';
				}
			}

			std::string newTexture(buff.begin(), buff.end());
			buff.clear();

			newTexture.erase(std::remove_if(newTexture.begin(), newTexture.end(), isspace), newTexture.end());
			newTexture.shrink_to_fit();

			texture = newTexture;

			buff = std::vector<char>(texture.begin(), texture.end());
			buff.resize(255);

			loadTexture(table, texture);
		}
	}
}
