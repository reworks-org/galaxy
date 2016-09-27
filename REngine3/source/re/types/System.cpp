//
//  System.cpp
//  REngine3
//
//  Created by reworks on 27/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "System.hpp"

namespace re
{
	System::~System()
	{
	}

	void System::RemoveEntity(sf::Uint64 e)
	{
		m_entitys.erase(e);
	}

	void System::AddEntity(Entity* e)
	{
		m_entitys.emplace(e->m_id, e);
	}

	std::unordered_map<sf::Uint64, Entity*>& System::GetEntitys()
	{
		return m_entitys;
	}
}