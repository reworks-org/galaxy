//
//  Level.hpp
//  REngine3
//
//  Created by reworks on 24/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_LEVEL_HPP_
#define RENGINE3_LEVEL_HPP_

#include <string>
#include <unordered_map>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "re/mapping/TMXManager.hpp"

namespace sf
{
	class Event;
	class Time;
}

namespace re
{
	class Window;

	class Level : public sf::Drawable
	{
	public:
		/*
		* IMPORTS: script - The name of the script in the VFS that defines the level..
		* EXPORTS: none
		* PURPOSE: Constructs the level using data from the lua script provided.
		*/
		Level(const std::string& script, sf::View camera);

		/*
		* IMPORTS: x & y values to move the level by
		* EXPORTS: none
		* PURPOSE: move the level.
		*/
		void Move(float x, float y);

		/*
		* IMPORTS: name of map to change to
		* EXPORTS: none
		* PURPOSE: Change the current map.
		*/
		void ChangeCurrentMap(const std::string& map);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process level events.
		*/
		virtual void Event(sf::Event& e) = 0;

		/*
		* IMPORTS: delta time.
		* EXPORTS: none
		* PURPOSE: Update state.
		*/
		virtual void Update(sf::Time dt) = 0;

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	protected:
		Window* m_window;
		sf::View m_camera;
		tmx::UPtr m_map;
	};
}

#endif