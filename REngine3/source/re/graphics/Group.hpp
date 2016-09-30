//
//  Group.hpp
//  REngine3
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_GROUP_HPP_
#define RENGINE3_GROUP_HPP_

#include <vector>

#include <SFML/Graphics/Drawable.hpp>

#include "re/component/TextComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"

#include "re/entity/Entity.hpp"

namespace re
{
	class Group : public sf::Drawable
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the entitys.
		*/
		~Group() override;

		/*
		* IMPORTS: TextComponent
		* EXPORTS: none
		* PURPOSE: Add a textocomponent to the group.
		*/
		void AddDrawable(sf::Uint64 id, std::shared_ptr<sf::Drawable> drawable);

		/*
		* IMPORTS: animated object
		* EXPORTS: none
		* PURPOSE: Add an animated object to the group.
		*/
		void AddAnimated(sf::Uint64 id, std::shared_ptr<Animated> animated);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Updates components in group.
		*/
		void Update(sf::Time dt);
		
		/*
		* IMPORTS: RenderTarget and RenderState
		* EXPORTS: none
		* PURPOSE: Inherited function. Not called by you, called by SFML internally.
		*/
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get stored animated.
		*/
		std::map<sf::Uint64, std::shared_ptr<Animated>>& GetAnimatedMap();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get stored drawables.
		*/
		std::map<sf::Uint64, std::shared_ptr<sf::Drawable>>& GetDrawableMap();

	private:
		std::map<sf::Uint64, std::shared_ptr<Animated>> m_animated;
		std::map<sf::Uint64, std::shared_ptr<sf::Drawable>> m_drawable;
	};
}

#endif