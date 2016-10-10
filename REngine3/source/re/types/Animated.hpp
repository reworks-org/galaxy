//
//  Animated.hpp
//  REngine3
//
//  Created by reworks on 22/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ANIMATED_HPP_
#define RENGINE3_ANIMATED_HPP_

#include <SFML/Graphics/Drawable.hpp>

namespace sf
{
	class Time;
}

namespace re
{
	class Animated : public sf::Drawable
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Abstract virtual destructor for animated classes.
		*/
		virtual ~Animated() {}

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update the animation.
		*/
		virtual void Update(sf::Time dt) = 0;

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {};
	};
}

#endif