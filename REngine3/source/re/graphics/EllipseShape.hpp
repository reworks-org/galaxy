//
//  EllipseShape.hpp
//  REngine3
//
//  Created on 31/10/2016.
//
// Thanks to: http://www.sfml-dev.org/tutorials/2.4/graphics-shape.php

#ifndef RENGINE3_ELLIPSESHAPE_HPP_
#define RENGINE3_ELLIPSESHAPE_HPP_

#include <SFML/Graphics/Shape.hpp>

namespace re
{
	class EllipseShape : public sf::Shape
	{
	public:
		explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0));

		void setRadius(const sf::Vector2f& radius);
		const sf::Vector2f& getRadius() const;
		unsigned int getPointCount() const override;
		sf::Vector2f getPoint(unsigned int index) const override;

	private:
		sf::Vector2f m_radius;
	};
}

#endif