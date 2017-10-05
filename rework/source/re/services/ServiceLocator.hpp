//
//  ServiceLocator.hpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_SERVICELOCATOR_HPP_
#define RENGINE3_SERVICELOCATOR_HPP_

#include <typeindex>
#include <unordered_map>

#include "re/utility/Log.hpp"
#include "re/types/Service.hpp"

namespace re
{
	class Locator
	{
	public:
		///
		/// Destructor
		///
		~Locator();

		///
		/// \brief To add a service to the engine.
		///
		/// A service is anything that inherits from Service base class.
		/// The locator lets them be accessed from anywhere in the engine.
		///
		/// \param s The type of object, s - The service object reference.
		///
		template<typename T>
		static void provide(Service* s);

		///
		/// To retrieve an engine service.
		///
		/// \return Service cast to correct type.
		///
		template<typename T>
		static T* get();

		///
		/// To remove a service from the locator (does not delete the original object, that is handled by Application class).
		///
		/// \param T The type of the object is used to identify which object to remove.
		///
		template<typename T>
		static void remove();

	private:
		static std::unordered_map<std::type_index, Service*> m_services;

	};

	template<typename T>
	void Locator::provide(Service* s)
	{
		// Find type in the map.
		auto it = m_services.find(std::type_index(typeid(T)));

		if (it != m_services.end()) 
		{
			RE_LOG(LogLevel::WARNING, "Tried to provide an already existing service.", "Locator::provide", "ServiceLocator.hpp", 68);
		}
		else 
		{
			m_services[typeid(T)] = s;
		}
	}

	template<typename T>
	T* Locator::get()
	{
		T* out;
		auto it = m_services.find(std::type_index(typeid(T)));

		out = it != m_services.end() ? dynamic_cast<T*>(it->second) : nullptr;
		return out;
	}

	template<typename T>
	void Locator::remove()
	{
		if (m_services.find(std::type_index(typeid(T))) != m_services.end())
		{
			m_services.erase(typeid(T));
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Attempted to remove a service that doesnt exist", "Locator::remove", "ServiceLocator.hpp", 99);
		}
	}
}

#endif
