//
//  ServiceLocator.hpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright © 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SERVICELOCATOR_HPP_
#define RENGINE3_SERVICELOCATOR_HPP_

#include <typeindex>
#include <unordered_map>

#include "re/utils/Log.hpp"
#include "re/Services/Service.hpp"

namespace re
{
	class Locator
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the locator.
		*/
		~Locator();

		/*
		* IMPORTS: <T> - The type of object, s - The service object reference.
		* EXPORTS: none
		* PURPOSE: To add an engine service to the engines locator.
		*/
		template<typename T>
		static void Provide(Service* s);

		/*
		* IMPORTS: <T> - The type of object
		* EXPORTS: none
		* PURPOSE: To retrieve an engine service.
		*/
		template<typename T>
		static T* Get();

		/*
		* IMPORTS: <T> - The type of object
		* EXPORTS: none
		* PURPOSE: To remove a service from the locator (does not delete the original object, that is handled by Application class).
		*/
		template<typename T>
		static void Remove();

	private:
		static std::unordered_map<std::type_index, Service*> m_Services;

	};

	template<typename T>
	void Locator::Provide(Service* s)
	{
		// Find type in the map.
		auto it = m_Services.find(std::type_index(typeid(T)));

		if (it != m_Services.end()) 
		{
			RE_LOG(LogLevel::WARNING, "Tried to provide an already existing service");
		}
		else 
		{
			m_Services[typeid(T)] = s;
		}
	}

	template<typename T>
	T* Locator::Get()
	{
		// Find type in the map.
		auto it = m_Services.find(std::type_index(typeid(T)));

		if (it != m_Services.end()) 
		{
			// Cast to required type and return.
			return dynamic_cast<T*>(it->second);
		}
		else 
		{
			return nullptr;
		}
	}

	template<typename T>
	void Locator::Remove()
	{
		// Find type in the map.
		auto it = m_Services.find(std::type_index(typeid(T)));

		if (it != m_Services.end())
		{
			// Erase from map.
			m_Services.erase(typeid(T));
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Attempted to remove a service that doesnt exist");
		}
	}
}

#endif