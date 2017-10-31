//
//  State.hpp
//  rework
//
//  Created by reworks on 27/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_STATE_HPP_
#define REWORK_STATE_HPP_

#include <memory>

union ALLEGRO_EVENT;

namespace re
{
	class BaseState
	{
	public:
		///
		/// Destructor.
		///
		virtual inline ~BaseState() {};

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
	};

	template<typename Derived>
	class State : public BaseState
	{
	public:
		///
		/// Destructor.
		///
		virtual inline ~State() {};

		///
		/// Retrieve instance of state.
		///
		inline static std::shared_ptr<Derived> inst()
		{
			static std::shared_ptr<Derived> m_statePtr = std::make_shared<Derived>();
			return m_statePtr;
		}

	protected:
		/// Allows for loading stuff only once in load()
		bool m_doneOnce = false;
	};
}

#endif