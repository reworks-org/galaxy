///
///  State.hpp
///  starlight
///
///  Created by reworks on 27/09/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STATE_HPP_
#define STARLIGHT_STATE_HPP_

#include <memory>

union ALLEGRO_EVENT;

namespace sl
{
	class BaseState
	{
	public:
		///
		/// Default constructor.
		///
		BaseState() = default;

		///
		/// Copy Constructor.
		///
		BaseState(const BaseState&) = default;

		///
		/// Move Constructor.
		///
		BaseState(BaseState&&) = default;

		///
		/// Destructor.
		///
		virtual ~BaseState() = default;

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
		virtual void update(const double dt) = 0;

		///
		/// Render the current state.
		///
		virtual void render() = 0;
	};

	template<typename Derived>
	class State : public BaseState
	{
		friend Derived;
		friend BaseState;
	public:
		///
		/// Destructor.
		///
		virtual ~State() = default;

		///
		/// Retrieve instance of state.
		///
		inline static std::shared_ptr<Derived> inst()
		{
			static std::shared_ptr<Derived> m_statePtr = std::make_shared<Derived>();
			return m_statePtr;
		}

	private:
		/// Allows for loading stuff only once in load()
		bool m_doneOnce = false;

	private:
		///
		/// Default constructor.
		///
		State() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		State(const State&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		State(State&&) = delete;
	};
}

#endif