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
	SpriteComponent::SpriteComponent()
	{
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::Init(sol::table& table)
	{
		m_group = table.get<sf::Uint32>("group");

		m_textureStream.open(table.get<std::string>("texture"));
		m_texture.loadFromStream(m_textureStream);
		setTexture(m_texture);
	}

	void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_vertices, 4, sf::TriangleStrip, states);
	}
}