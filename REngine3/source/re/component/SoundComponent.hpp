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
#include "re/services/vfs/sfmlphysfs.hpp"

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
		void Init(sol::table& table) override;

	private:
		std::unordered_map<std::string, std::pair<std::pair<std::unique_ptr<sf::SoundBuffer>, std::unique_ptr<sf::physfs>>, std::unique_ptr<sf::Sound>>> m_sounds;
	};
}

#endif