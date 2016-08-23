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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

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
		void AddTextComponent(std::shared_ptr<TextComponent> tc);

		/*
		* IMPORTS: SpriteComponent
		* EXPORTS: none
		* PURPOSE: Add a textocomponent to the group.
		*/
		void AddSpriteComponent(std::shared_ptr<SpriteComponent> sc);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Updates components in group.
		*/
		void Update();

		/*
		* IMPORTS: RenderTarget and RenderState
		* EXPORTS: none
		* PURPOSE: Inherited function. Not called by you, called by SFML internally.
		*/
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		std::vector<std::shared_ptr<TextComponent>> m_tc;
		std::vector<std::shared_ptr<SpriteComponent>> m_sc;
	};
}

#endif