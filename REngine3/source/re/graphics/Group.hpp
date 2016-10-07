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
		* IMPORTS: id and drawable object
		* EXPORTS: none
		* PURPOSE: add a drawable entity's components to the group.
		*/
		void AddDrawable(sf::Uint64 id, std::shared_ptr<sf::Drawable> drawable);

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
		* PURPOSE: Get stored drawables.
		*/
		std::map<sf::Uint64, std::shared_ptr<sf::Drawable>>& GetDrawableMap();

	private:
		std::map<sf::Uint64, std::shared_ptr<sf::Drawable>> m_drawable;
	};
}

#endif