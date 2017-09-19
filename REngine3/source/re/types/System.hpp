//
//  System.hpp
//  REngine3
//
//  Created by reworks on 10/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_SYSTEM_HPP_
#define RENGINE3_SYSTEM_HPP_

#include <map>

#include "re/core/Entity.hpp"

namespace re
{    
	class System
	{
	public:
		///
		/// Virtual destructor for systems.
		///
		virtual ~System();

		///
		/// Add an entitys components from the system. 
		///
		virtual void addEntity(Entity* e) = 0;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		virtual void removeEntity(const std::string& name) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		virtual void clean() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get stored components.
		*/
		std::map<std::string, Entity*>& getEntitys();

		/*
		* IMPORTS: none
		* EXPORTS: std::string CONSTANT
		* PURPOSE: Get the type of the system as a std::string.
		*/
		std::string getTypeAsString() const;

	protected:
		std::map<std::string, Entity*> m_entitys;
		std::string m_typeAsString;
	};
}

#endif
