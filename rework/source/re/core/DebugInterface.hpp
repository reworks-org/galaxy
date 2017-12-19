///
///  DebugInterface.hpp
///  rework
///
///  Created by reworks on 20/04/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_DEBUGINTERFACE_HPP_
#define REWORK_DEBUGINTERFACE_HPP_

#include <functional>

#include "re/types/ServiceLocator.hpp"

union ALLEGRO_EVENT;
struct ALLEGRO_DISPLAY;

namespace re
{
	class BaseState; 
    class DebugInterface : public ServiceLocator<DebugInterface>
    {
    public:
		///
		/// Constructor.
		///
		/// \param display ALLEGRO_DISPLAY object.
		///
		DebugInterface(ALLEGRO_DISPLAY* display);

		///
		/// Destructor.
		///
		~DebugInterface() override;

		///
		/// Set if debug manager is disabled...
		///
		/// \param isDisabled Boolean. Set to true to disable.
		///
		void disable(bool isDisabled);

		///
		/// Process imgui events.
		///
		/// \param event ALLEGRO_EVENT object.
		///
		void event(ALLEGRO_EVENT* event);

		///
		/// Begin new imgui frame.
		///
		void newFrame();

		///
		/// Draw IMGUI.
		///
		void render();

		///
		/// \brief Display menu on screen.
		///
		/// Call between update and render. Calls the functions that make up the main debug menu. ImGui functions mainly.
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
		void specifyReloadState(std::shared_ptr<BaseState> s, std::function<void(void)> func);

	private:
		///
		/// Copy Constructor.
		/// Deleted.
		///
		DebugInterface(const DebugInterface&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		DebugInterface(DebugInterface&&) = delete;

	private:
		bool m_disabled;
		std::function<void(void)> m_reloadFunc;
		std::shared_ptr<BaseState> m_reloadState;
    };
}

#endif