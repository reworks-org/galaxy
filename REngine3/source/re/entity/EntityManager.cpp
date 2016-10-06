//
//  EntityManager.cpp
//  REngine3
//
//  Created by reworks on 27/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "EntityManager.hpp"

namespace re
{
	EntityManager::~EntityManager()
	{
		m_entitys.clear();
	}

	void EntityManager::Add(const std::string& name, Entity* e)
	{
		m_entitys.emplace(name, e);
	}

	Entity* EntityManager::At(const std::string& name)
	{
		return m_entitys[name];
	}

	void EntityManager::Clean()
	{
		m_entitys.clear();
	}

	std::unordered_map<std::string, Entity*>& EntityManager::GetMap()
	{
		return m_entitys;
	}
}