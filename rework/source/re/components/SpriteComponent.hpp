//
//  SpriteComponent.hpp
//  rework
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SPRITECOMPONENT_HPP_
#define REWORK_SPRITECOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "re/types/Renderable.hpp"

namespace re
{
	class SpriteComponent : public Renderable
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SpriteComponent(ex::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~SpriteComponent() override;
		
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	private:
		///
		/// Render sprite. Called by rendersystem.
		///
		void render() override;

	public:
		std::string m_spriteName;
	};
}

#endif