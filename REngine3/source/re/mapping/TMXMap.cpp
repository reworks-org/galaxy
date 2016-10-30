//
//  TMXMap.cpp
//  REngine3
//
//  Created by reworks on 30/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "TMXMap.hpp"
#include <SFML/Graphics/VertexArray.hpp>
namespace re
{
	TMXMap::TMXMap(const std::string& script)
	{
		m_primitiveType = sf::Quads;
		m_vertices.resize(4);

		// lua crap
	}

	void TMXMap::Update(sf::Time dt)
	{

	}

	void TMXMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		states.shader = &m_shader;

		target.draw(&m_vertices[0], m_vertices.size(), m_primitiveType, states);
	}
}