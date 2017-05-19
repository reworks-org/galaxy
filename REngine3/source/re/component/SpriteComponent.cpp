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

#include "re/debug/imgui/imgui.h"
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

	bool SpriteComponent::debugFunction(sol::table& table)
	{
		bool saveData = false;

		ImGui::Spacing();
		ImGui::Text(std::string("Group: " + std::to_string(m_group)).c_str());

		static std::string original = table.get<std::string>("texture");
		static std::string newTextureAlt = "";
		static std::vector<char> buff(original.begin(), original.end());
		static bool doneOnce = false;

		if (!doneOnce)
		{
			buff.resize(255);
			doneOnce = true;
		}
		
		// fix this
		if (original != table.get<std::string>("texture") && original != newTextureAlt)
		{
			original = table.get<std::string>("texture");
			buff.clear();
			buff = std::vector<char>(original.begin(), original.end());
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

			original = newTexture;
			newTextureAlt = newTexture;

			buff = std::vector<char>(original.begin(), original.end());
			buff.resize(255);

			printf(newTexture.c_str());
			table.set("texture", newTexture);
			loadTexture(table, newTexture);
		}
		
		if (ImGui::Button("Write out changes?"))
		{
			saveData = true;
		}
		
		return saveData;
	}

	void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_vertices, 4, sf::TriangleStrip, states);
	}
}
