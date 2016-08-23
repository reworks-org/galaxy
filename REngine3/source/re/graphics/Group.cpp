//
//  Group.cpp
//  REngine3
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Group.hpp"

namespace re
{
	Group::~Group()
	{
		m_tc.clear();
		m_sc.clear();
	}

	void Group::AddTextComponent(std::shared_ptr<TextComponent> tc)
	{
		m_tc.push_back(tc);
	}

	void Group::AddSpriteComponent(std::shared_ptr<SpriteComponent> sc)
	{
		m_sc.push_back(sc);
	}

	void Group::Update()
	{
	}

	void Group::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		for (auto& sc : m_sc)
		{
			if (sc->Shader() != nullptr)
			{
				target.draw(sc->m_sprite, sc->Shader());
			}
			else
			{
				target.draw(sc->m_sprite);
			}
		}

		for (auto& tc : m_tc)
		{
			target.draw(tc->m_text);
		}
	}
}