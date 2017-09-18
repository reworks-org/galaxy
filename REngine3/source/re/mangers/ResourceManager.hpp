//
//  ResourceManager.hpp
//  REngine3
//
//  Created by reworks on 11/06/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_RESOURCEMANAGER_HPP_
#define RENGINE3_RESOURCEMANAGER_HPP_

#include <map>
#include <memory>
#include <string>

#include "re/services/vfs.hpp"
#include "re/types/Service.hpp"
#include "re/services/ServiceLocator.hpp"

namespace re
{
	template<typename Resource>
	class ResourceManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleanup manager.
		*/
		~ResourceManager() override;

		/*
		* IMPORTS: fileName - name of file in vfs to add, id - id for retrieveal purposes.
		* EXPORTS: none
		* PURPOSE: Add an item to the resource mananger.
		*/
		void add(const std::string& fileName, const std::string& id, const std::string& str);

		/*
		* IMPORTS: fileName - name of file in vfs to add, id - id for retrieveal purposes.
		* EXPORTS: none
		* PURPOSE: Add an item to the resource mananger.
		*/
		void add(const std::string& fileName, const std::string& id);

		/*
		* IMPORTS: id - id of object to retrieve.
		* EXPORTS: T& object.
		* PURPOSE: Retrieve an object from the map.
		*/
		Resource& get(const std::string& id);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the internal map up.
		*/
		void clean();

	private:
		std::map<std::string, std::unique_ptr<Resource>> m_resourceMap;
	};

	template<typename Resource>
	ResourceManager<Resource>::~ResourceManager()
	{
		m_resourceMap.clear();
	}

	template<typename Resource>
	void ResourceManager<Resource>::add(const std::string& fileName, const std::string& id)
	{
		m_resourceMap.emplace(id, std::make_unique<Resource>());
		
		m_resourceMap[id]->loadFromFile(Locator::get<VFS>()->retrieve(fileName));
	}

	template<typename Resource>
	void ResourceManager<Resource>::add(const std::string& fileName, const std::string& id, const std::string& str)
	{
		m_resourceMap.emplace(id, std::make_unique<Resource>());

		m_resourceMap[id]->loadFromFile(Locator::get<VFS>()->retrieve(fileName), Locator::get<VFS>()->retrieve(str));
	}

	template<typename Resource>
	Resource& ResourceManager<Resource>::get(const std::string& id)
	{
		return *(m_resourceMap[id]);
	}

	template<typename Resource>
	void ResourceManager<Resource>::clean()
	{
		m_resourceMap.clear();
	}
}

#endif
