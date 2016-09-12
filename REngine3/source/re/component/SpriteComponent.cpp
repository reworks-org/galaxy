//
//  SpriteComponent.cpp
//  REngine3
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent(sol::table& table)
	{
		setPosition({ table.get<float>("x"), table.get<float>("y")});
		m_group = table.get<unsigned long>("group");

		LoadTexture(table.get<std::string>("texture"));
		
		LoadShader(table.get<std::string>("vert"), table.get<std::string>("frag"));
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	sf::Shader* SpriteComponent::Shader()
	{
		return m_shader.get();
	}

	void SpriteComponent::LoadTexture(const std::string& texture)
	{
		m_textureStream.open(texture);
		m_texture.loadFromStream(m_textureStream);
		setTexture(m_texture);
	}

	void SpriteComponent::LoadShader(const std::string& vert, const std::string& frag)
	{
		m_shader = std::make_shared<sf::Shader>();

		if ((vert != "null") && (frag == "null"))
		{
			m_vertStream.open(vert);
			m_shader->loadFromStream(m_vertStream, sf::Shader::Vertex);
		}
		else if ((vert == "null") && (frag != "null"))
		{
			m_fragStream.open(frag);
			m_shader->loadFromStream(m_fragStream, sf::Shader::Fragment);
		}
		else if ((vert != "null") && (frag != "null"))
		{
			m_vertStream.open(vert);
			m_fragStream.open(frag);
			m_shader->loadFromStream(m_vertStream, m_fragStream);
		}
		else
		{
			m_shader = nullptr;
		}
	}

	void SpriteComponent::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;

		if (m_shader != nullptr)
		{
			states.shader = m_shader.get();
		}

		target.draw(m_vertices, 4, sf::TriangleStrip, states);
	}
}