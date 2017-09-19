//
//  SoundComponent.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SOUNDCOMPONENT_HPP_
#define RENGINE3_SOUNDCOMPONENT_HPP_

#include <unordered_map>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "re/types/Component.hpp"

namespace re
{
	class SoundComponent : public Component
	{
		friend class AudioSystem;
        
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		SoundComponent();
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~SoundComponent() override;

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
		std::unordered_map<std::string, std::pair<sf::SoundBuffer, std::unique_ptr<sf::Sound>>> m_sounds;
	};
}

#endif
