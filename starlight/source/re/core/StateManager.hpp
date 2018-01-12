///
///  StateManager.hpp
///  starlight
///
///  Created by reworks on 5/10/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_STATEMANAGER_HPP_
#define REWORK_STATEMANAGER_HPP_

#include <memory>

#include "re/types/ServiceLocator.hpp"

namespace sl
{
	class BaseState;
	class StateManager final : public ServiceLocator<StateManager>
	{
	public:
		///
		/// Constructor.
		///
		StateManager() = default;

		///
		/// Destructor.
		///
		~StateManager() override;

		///
		/// Load the states resources.
		///
		void load();

		///
		/// Unload the states resources.
		///
		void unload();
	    
        ///
		/// Poll for events in the current state.
		///
		void event(ALLEGRO_EVENT* event);
        
		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(double dt);

		///
		/// Render the current state.
		///
		void render();

		///
		/// \brief Change the current state.
		///
		/// Calls unload then load().
		///
		/// \param s Pointer to new state.
		///
		void changeState(std::shared_ptr<BaseState> s);

		///
		/// \brief Reloads the current state.
		///
		/// Calls unload then load().
		///
		/// \param s Optional pointer to a state to reload to.
		///
		void reloadState(std::shared_ptr<BaseState> s = nullptr);

		///
		/// \brief Set a new active state.
		///
		/// This changes the state but WITHOUT CHANGING RESOURCES!
		///
		/// \param s Pointer to new state.
		///
		void setState(std::shared_ptr<BaseState> s);
			    
	private:
		///
		/// Copy Constructor.
		/// Deleted.
		///
		StateManager(const StateManager&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		StateManager(StateManager&&) = delete;


	private:
	    std::shared_ptr<BaseState> m_currentState = nullptr;
	};
}

#endif