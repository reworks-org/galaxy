//
//  SpriteComponent.cpp
//  REngine3
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent(sol::table& table)
	{
		m_sprite.setPosition({ table.get<float>("x"), table.get<float>("y")});
		m_group = table.get<unsigned long>("group");

		textureData.open(table.get<std::string>("texture"));
		m_texture.loadFromStream(textureData);

		m_sprite.setTexture(m_texture);

		if ((table.get<std::string>("vert") != "null") && (table.get<std::string>("frag") == "null"))
		{
			shaderData.open(table.get<std::string>("vert"));
			m_shader->loadFromStream(shaderData, sf::Shader::Vertex);
		}
		else if ((table.get<std::string>("vert") == "null") && (table.get<std::string>("frag") != "null"))
		{
			shaderData.open(table.get<std::string>("frag"));
			m_shader->loadFromStream(shaderData, sf::Shader::Fragment);
		}
		else if ((table.get<std::string>("vert") != "null") && (table.get<std::string>("frag") != "null"))
		{
			m_shader->loadFromMemory(Locator::Get<VFS>()->ToString(table.get<std::string>("vert")), Locator::Get<VFS>()->ToString(table.get<std::string>("frag")));
		}
		else
		{
			m_shader = nullptr;
		}
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	sf::Shader* SpriteComponent::Shader()
	{
		return m_shader.get();
	}
}