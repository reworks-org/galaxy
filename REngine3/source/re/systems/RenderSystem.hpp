//
//  RenderSystem.hpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_RENDERSYSTEM_HPP_
#define RENGINE3_RENDERSYSTEM_HPP_

#include "re/types/System.hpp"
#include "re/graphics/Group.hpp"

namespace re
{
	class Window;

	class RenderSystem : public System
	{
	public:
		/*
		* IMPORTS: Number of groups. Think "layers". Each group is a layer of entitys to render.
		* EXPORTS: none
		* PURPOSE: Construct the system.
		*/
		RenderSystem(int numOfGroups);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the renderer.
		*/
		~RenderSystem() override;

		/*
		* IMPORTS: world - the World* object.
		* EXPORTS: none
		* PURPOSE: Submit all entitys to the system.
		*/
		void Submit(re::World* world);
		
		/*
		* IMPORTS: group and drawable object
		* EXPORTS: none
		* PURPOSE: Submit an entity with a custom drawable component.
		*/
		void Submit(unsigned long group, std::shared_ptr<sf::Drawable> drawable);

		/*
		* IMPORTS: group and animated object
		* EXPORTS: none
		* PURPOSE: Submit an entity with a custom animated component.
		*/
		void Submit(unsigned long group, std::shared_ptr<Animated> animated);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update the system. Mainly for animations
		*/
		void Update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the entitys.
		*/
		void Render(re::Window* window);

	private:
		std::vector<Group> m_groups;
	};
}

#endif