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
	: m_vertices(sf::Quads, 4)
	{
		m_group = table["group"];

		sol::table position = table["position"];

		m_vertices[0].position = sf::Vector2f(position["Vertex0"]["x"], position["Vertex0"]["y"]);
		m_vertices[1].position = sf::Vector2f(position["Vertex1"]["x"], position["Vertex1"]["y"]);
		m_vertices[2].position = sf::Vector2f(position["Vertex2"]["x"], position["Vertex2"]["y"]);
		m_vertices[3].position = sf::Vector2f(position["Vertex3"]["x"], position["Vertex3"]["y"]);

		sol::table texCoords = table["texCoords"];

		m_vertices[0].texCoords = sf::Vector2f(texCoords["Vertex0"]["x"], texCoords["Vertex0"]["y"]);
		m_vertices[1].texCoords = sf::Vector2f(texCoords["Vertex1"]["x"], texCoords["Vertex1"]["y"]);
		m_vertices[2].texCoords = sf::Vector2f(texCoords["Vertex2"]["x"], texCoords["Vertex2"]["y"]);
		m_vertices[3].texCoords = sf::Vector2f(texCoords["Vertex3"]["x"], texCoords["Vertex3"]["y"]);

		std::string textureBuffer = Locator::Get<VFS>()->ToString(table["texture"]);
		m_texture.loadFromMemory(textureBuffer.c_str(), textureBuffer.size());
		
		if ((table["vert"] != "null") && (table["frag"] == "null"))
		{
			m_shader.loadFromMemory(Locator::Get<VFS>()->ToString(table["vert"]), sf::Shader::Vertex);
		}
		else if ((table["vert"] == "null") && (table["frag"] != "null"))
		{
			m_shader.loadFromMemory(Locator::Get<VFS>()->ToString(table["frag"]), sf::Shader::Fragment);
		}
		else if ((table["vert"] != "null") && (table["frag"] != "null"))
		{
			m_shader.loadFromMemory(Locator::Get<VFS>()->ToString(table["vert"]), Locator::Get<VFS>()->ToString(table["frag"]));
		}
	}

	SpriteComponent::~SpriteComponent()
	{
		m_vertices.clear();
	}

	void SpriteComponent::Update(sf::Vector2f pos)
	{
		m_transform.setPosition(pos);
	}
}