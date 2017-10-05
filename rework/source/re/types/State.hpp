//
//  State.hpp
//  REngine3
//
//  Created by reworks on 27/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_STATE_HPP_
#define RENGINE3_STATE_HPP_

union ALLEGRO_EVENT;
struct ALLEGRO_TIMER;

namespace re
{
	class VFS;
	class World;
	class Window;
	class FontManager;
	class ConfigReader;
	class StateManager;

	class State
	{
	public:
		///
		/// Destructor.
		///
		virtual ~State() {};

		///
		/// Load the states resources.
		///
		virtual void load() = 0;

		///
		/// Unload the states resources.
		///
		virtual void unload() = 0;

		///
		/// Handle any events for this state.
		///
		virtual void event(ALLEGRO_EVENT* event) = 0;

		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		virtual void update(ALLEGRO_TIMER* dt) = 0;

		///
		/// Render the current state.
		///
		virtual void render() = 0;

	protected:
		// Services
		Window* m_window;
		World* m_world;
		VFS* m_vfs;
		StateManager* m_stateManager;
		FontManager* m_fontManager;

		// Allows for loading stuff only once in load()
		bool m_doneOnce = false;
	};
}

#endif