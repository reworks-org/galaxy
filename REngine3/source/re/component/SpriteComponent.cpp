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

#include "re/debug/imgui/imgui-sfml.h"
#include "re/services/vfs/VFS.hpp"
#include "re/graphics/TextureAtlas.hpp"
#include "re/services/ServiceLocator.hpp"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent()
	{
        m_group = 0;
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::init(sol::table& table)
	{
		m_group = table.get<sf::Uint32>("group");
        
		loadTexture(table);
	}

	void SpriteComponent::loadTexture(sol::table& table, const std::string& texture)
	{
		if (table.get<bool>("isSpriteSheet") == true)
		{
			if (texture == "")
			{
				setTexture(Locator::get<TextureAtlas>()->get(table.get<std::string>("altas_ref")));
			}
			else
			{
				setTexture(Locator::get<TextureAtlas>()->get(table.get<std::string>(texture)));
			}
			sol::table rect = table.get<sol::table>("rect");
			setTextureRect(sf::IntRect(rect.get<int>("tx"), rect.get<int>("ty"), rect.get<int>("tw"), rect.get<int>("th")));
		}
		else
		{
			if (texture == "")
			{
				// NEED TO CHECK IF FILE IS VALID, THEN CLOSE THE ORIGINAL BEFORE OPENING A NEW ONE!
				m_textureStream.open(table.get<std::string>("texture"));
			}
			else
			{
				m_textureStream.open(texture);
			}
			m_texture.loadFromStream(m_textureStream);
			setTexture(m_texture);
		}
	}

	void SpriteComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Text(std::string("Group: " + std::to_string(m_group)).c_str());

		static std::string input = table.get<std::string>("texture");
		static std::string newTextureAlt = "";
		static std::vector<char> buff(input.begin(), input.end());
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
			input = table.get<std::string>("texture");
			buff.clear();
			buff = std::vector<char>(input.begin(), input.end());
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

			input = newTexture;
			newTextureAlt = newTexture;

			buff = std::vector<char>(input.begin(), input.end());
			buff.resize(255);

			loadTexture(table, newTexture);
		}
	}

	void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_vertices, 4, sf::TriangleStrip, states);
	}
}
