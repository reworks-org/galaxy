//
//  State.hpp
//  rework
//
//  Created by reworks on 27/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_STATE_HPP_
#define REWORK_STATE_HPP_

union ALLEGRO_EVENT;

namespace re
{
	class State
	{
	public:
		///
		/// Destructor.
		///
		virtual inline ~State() {};

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
		virtual void update(double dt) = 0;

		///
		/// Render the current state.
		///
		virtual void render() = 0;

	protected:
		// Allows for loading stuff only once in load()
		bool m_doneOnce = false;
	};
}

#endif