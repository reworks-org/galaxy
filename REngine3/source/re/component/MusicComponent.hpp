//
//  MusicComponent.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_MUSICCOMPONENT_HPP_
#define RENGINE3_MUSICCOMPONENT_HPP_

#include <unordered_map>

#include <SFML/Audio/Music.hpp>

#include "re/types/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class MusicComponent : public Component
	{
		friend class AudioSystem;
        typedef std::unordered_map<std::string, std::pair<std::unique_ptr<sf::physfs>, std::unique_ptr<sf::Music>>> MusicStorage;
        
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		MusicComponent();
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~MusicComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: lua table and entity name
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table, const std::string& curEntityName) override;
	
	private:
		MusicStorage m_music;
	};
}

#endif
