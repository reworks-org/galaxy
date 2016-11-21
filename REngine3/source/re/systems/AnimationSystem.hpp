//
//  AnimationSystem.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//  Code modified from: https://github.com/miguelmartin75/anax/blob/master/examples/common/include/Systems/AnimationSystem.hpp
//  See original file for details.

#ifndef RENGINE3_ANIMATIONSYSTEM_HPP_
#define RENGINE3_ANIMATIONSYSTEM_HPP_

#include "re/types/System.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	class AnimationSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the system.
		*/
		~AnimationSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world) override;

		/*
		* IMPORTS: pointer to entity to add.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(Entity* e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(const std::string& name) override;

		/*
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Update the animations in the system.
		*/
		void Update(sf::Time dt);

		/*
		* IMPORTS: fps of animations.
		* EXPORTS: none
		* PURPOSE: To control the speed of the animations.
		*/
		void SetFPS(unsigned int fps);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean() override;

	private:
		unsigned int m_fps;
	};
}

#endif