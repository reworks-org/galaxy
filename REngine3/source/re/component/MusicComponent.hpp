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

#include "re/audio/Music.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class MusicComponent : public Component
	{
		friend class AudioSystem;
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
		void Init(sol::table& table) override;

	private:
		std::unordered_map<std::string, std::shared_ptr<Music>> m_music;
	};
}

#endif