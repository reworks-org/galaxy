//
//  SoundComponent.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_SOUNDCOMPONENT_HPP_
#define RENGINE3_SOUNDCOMPONENT_HPP_

#include <unordered_map>

#include "re/types/Component.hpp"

namespace re
{
	class SoundComponent : public Component
	{
		friend class AudioSystem;
        
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SoundComponent(sol::table& table);
		
		///
		/// Destructor.
		///
		~SoundComponent() override;

	private:
		///
		/// Debug function used by IMGUI.
		///
		void debug(sol::table& table, const std::string& curEntityName) override;

	private:
		std::unordered_map<std::string, std::pair<sf::SoundBuffer, std::unique_ptr<sf::Sound>>> m_sounds;
	};
}

#endif
