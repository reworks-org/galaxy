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
        /*
         * IMPORTS: Radius of the ellipse. Default of 0.
         * EXPORTS: none
         * PURPOSE: Construct an ellipse with a specific radius.
         */
		explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0));
        
        /*
         * IMPORTS: radius - new radius of eclipse.
         * EXPORTS: none
         * PURPOSE: Change the radius of the ellipse.
         */
		void setRadius(const sf::Vector2f& radius);
        
        /*
         * IMPORTS: none
         * EXPORTS: const radius of ellipse.
         * PURPOSE: get the current radius of the eclipse.
         */
		const sf::Vector2f& getRadius() const;
        
        /*
         * IMPORTS: none
         * EXPORTS: std::size_t of point count.
         * PURPOSE: Get point count.
         */
        std::size_t getPointCount() const override;
        
        /*
         * IMPORTS: none
         * EXPORTS: vector2f of the point.
         * PURPOSE: Get a specific point from the ellipse.
         */
		sf::Vector2f getPoint(std::size_t index) const override;

	private:
		sf::Vector2f m_radius;
	};
}

#endif
