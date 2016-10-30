//
//  TMXMap.hpp
//  REngine3
//
//  Created by reworks on 30/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TMXMAP_HPP_
#define RENGINE3_TMXMAP_HPP_

#include <vector>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "re/mapping/TMXSmartPointer.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	class TMXMap : public sf::Drawable, public sf::Transformable
	{
	public:
		TMXMap(const std::string& script);

		void Update(sf::Time dt);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::PrimitiveType m_primitiveType;
		std::vector<sf::Vertex> m_vertices;

		sf::Shader m_shader;
		sf::Texture m_texture;
	};
}

#endif