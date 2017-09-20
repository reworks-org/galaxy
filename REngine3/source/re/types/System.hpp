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

		///
		/// Removes an entity from the system.
		///
		/// \param name The name of the entity to remove.
		///
		virtual void removeEntity(const std::string& name) = 0;

		///
		/// Cleans the system. Removes all attached entitys.
		///
		virtual void clean() = 0;

		///
		/// Function to get the std::map that stores the entitys.
		///
		/// \return Returns a std::map<std::string, Entity*> reference.
		///
		std::map<std::string, Entity*>& getEntitys();

		///
		/// \brief Get the type of the system as a string.
		/// 
		/// This is used so you can identify each system cross-platform.
		///
		/// \return Returns a CONSTANT std::string that you can't modify.
		///
		std::string& getTypeAsString() const;

	protected:
		std::map<std::string, Entity*> m_entitys;
		std::string m_typeAsString;
	};
}

#endif
