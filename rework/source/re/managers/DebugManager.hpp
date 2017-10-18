//
//  DebugManager.hpp
//  rework
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_DEBUGMANAGER_HPP_
#define REWORK_DEBUGMANAGER_HPP_

#include "sol2/sol.hpp"
#include "re/types/Service.hpp"

union ALLEGRO_EVENT;
struct ALLEGRO_DISPLAY;

namespace re
{
	class State;
	class World;

    class DebugManager : public Service
    {
    public:
		///
		/// Constructor.
		///
		/// \param display ALLEGRO_DISPLAY object.
		///
		DebugManager(ALLEGRO_DISPLAY* display);

		///
		/// Destructor.
		///
		~DebugManager() override;

		///
		/// Set if debug manager is disabled...
		///
		/// \param isDisabled Boolean. Set to true to disable.
		///
		void disable(bool isDisabled);

		///
		/// Process events.
		///
		/// \param event ALLEGRO_EVENT object.
		///
		void event(ALLEGRO_EVENT* event);

		///
		/// Update IMGUI.
		///
		void update();

		///
		/// Draw IMGUI.
		///
		void render();

		///
		/// \brief Display menu on screen.
		///
		/// Call between update and render. Calls the functions that make up the main debug menu.
		///
		void displayMenu();

		///
		/// \brief Set a state to reload too.
		///
		/// func() is called first when reloading.
		///
		/// \param s State pointer to reload to.
		/// \param func Function to cleanup anything that normally isnt cleaned up at that time.
		///
		void specifyReloadState(std::shared_ptr<State> s, std::function<void(void)> func);

	private:
		std::shared_ptr<State> m_reloadState;
		std::function<void(void)> m_reloadFunc;
		re::World* m_world;
		sol::state m_lua;
		bool m_disabled;
    };
}

#endif