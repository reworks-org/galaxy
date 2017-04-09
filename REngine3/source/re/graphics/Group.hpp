//
//  Group.hpp
//  REngine3
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_GROUP_HPP_
#define RENGINE3_GROUP_HPP_

#include <map>
#include <vector>
#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace re
{
	typedef std::vector<std::pair<std::shared_ptr<sf::Drawable>, std::shared_ptr<sf::Transformable>>> RenderableContainer;

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
		void addDrawable(const std::string& name, std::shared_ptr<sf::Drawable> drawable, std::shared_ptr<sf::Transformable> transformable);
		
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
		std::map<std::string, RenderableContainer>& getDrawableMap();

	private:
		std::map<std::string, RenderableContainer> m_drawable;
	};
}

#endif
