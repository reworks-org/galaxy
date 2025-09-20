///
/// StateMachine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_STATEMACHINE_HPP_
#define GALAXY_STATE_STATEMACHINE_HPP_

#include <ankerl/unordered_dense.h>

#include "galaxy/math/FNV1a.hpp"
#include "galaxy/state/State.hpp"
#include "galaxy/logging/Log.hpp"

namespace galaxy
{
	///
	/// \brief A finite state machine.
	///
	/// https://gameprogrammingpatterns.com/state.html
	///
	/// \tparam Stored Object type being used in the state machine.
	///
	template<std::derived_from<State> Stored>
	class StateMachine
	{
		///
		/// We specifically use a std::vector for our stack.
		///
		using Stack = std::vector<std::shared_ptr<Stored>>;

		///
		/// Use a dense map for storage.
		///
		using Map = ankerl::unordered_dense::map<std::uint64_t, std::shared_ptr<Stored>>;

	public:
		///
		/// Constructor.
		///
		StateMachine() noexcept;

		///
		/// Move constructor.
		///
		StateMachine(StateMachine<Stored>&&);

		///
		/// Move assignment operator.
		///
		StateMachine<Stored>& operator=(StateMachine<Stored>&&);

		///
		/// Destructor.
		///
		virtual ~StateMachine();

		///
		/// Add a new state.
		///
		/// \tparam Args Constructor arguments for state.
		///
		/// \param key Name to assign to this state.
		/// \param args Variable arguments for state constructor.
		///
		/// \return Shared pointer to added scene.
		///
		template<typename... Args>
		[[maybe_unused]]
		std::shared_ptr<Stored> add(const std::string& key, Args&&... args);

		///
		/// Get a specific state.
		///
		/// \param key Name of state.
		///
		/// \return Shared pointer to scene.
		///
		[[nodiscard]]
		std::shared_ptr<Stored> get(const std::string& key) noexcept;

		///
		/// Does a state exist.
		///
		/// \param key Name of state.
		///
		/// \return True if exists.
		///
		[[nodiscard]]
		bool has(const std::string& key) noexcept;

		///
		/// \brief Remove a specific state.
		///
		/// Will fail if state is in stack.
		///
		/// \param key Name of state.
		///
		void remove(const std::string& key);

		///
		/// Push a state onto the top of stack.
		///
		/// \param key Name of state.
		///
		void push(const std::string& key) noexcept;

		///
		/// Remove state on top of stack.
		///
		void pop() noexcept;

		///
		/// Remove all states in stack.
		///
		void pop_all() noexcept;

		///
		/// Get top state in stack.
		///
		/// \return Shared pointer to top state.
		///
		[[nodiscard]]
		std::shared_ptr<Stored> top() const noexcept;

		///
		/// Process events and updates.
		///
		virtual void update() = 0;

		///
		/// Get stack.
		///
		/// \return Reference.
		///
		[[nodiscard]]
		Stack& stack() noexcept;

		///
		/// Get storage.
		///
		/// \return Reference.
		///
		[[nodiscard]]
		Map& storage() noexcept;

	protected:
		///
		/// State stack.
		///
		Stack m_stack;

		///
		/// State cache.
		///
		Map m_storage;

	private:
		///
		/// Copy constructor.
		///
		StateMachine(const StateMachine<Stored>&) = delete;

		///
		/// Copy assignment operator.
		///
		StateMachine<Stored>& operator=(const StateMachine<Stored>&) = delete;
	};

	template<std::derived_from<State> Stored>
	StateMachine<Stored>::StateMachine() noexcept
	{
	}

	template<std::derived_from<State> Stored>
	inline StateMachine<Stored>::StateMachine(StateMachine<Stored>&& sm)
	{
		this->m_stack   = std::move(sm.m_stack);
		this->m_storage = std::move(sm.m_storage);
	}

	template<std::derived_from<State> Stored>
	inline StateMachine<Stored>& StateMachine<Stored>::operator=(StateMachine<Stored>&& sm)
	{
		if (this != &sm)
		{
			this->m_stack   = std::move(sm.m_stack);
			this->m_storage = std::move(sm.m_storage);
		}

		return *this;
	}

	template<std::derived_from<State> Stored>
	inline StateMachine<Stored>::~StateMachine()
	{
		pop_all();

		m_stack.clear();
		m_storage.clear();
	}

	template<std::derived_from<State> Stored>
	template<typename... Args>
	inline std::shared_ptr<Stored> StateMachine<Stored>::add(const std::string& key, Args&&... args)
	{
		const auto hash = math::fnv1a(key.c_str());

		if (!m_storage.contains(hash))
		{
			m_storage[hash] = std::make_shared<Stored>(key, std::forward<Args>(args)...);
			return m_storage[hash];
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to create a scene with a duplicate key of '{0}'.", key);
			return nullptr;
		}
	}

	template<std::derived_from<State> Stored>
	inline std::shared_ptr<Stored> StateMachine<Stored>::get(const std::string& key) noexcept
	{
		const auto hash = math::fnv1a(key.c_str());

		if (m_storage.contains(hash))
		{
			return m_storage[hash];
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "State '{0}' does not exist.", key);
			return nullptr;
		}
	}

	template<std::derived_from<State> Stored>
	inline bool StateMachine<Stored>::has(const std::string& key) noexcept
	{
		const auto hash = math::fnv1a(key.c_str());
		return m_storage.contains(hash);
	}

	template<std::derived_from<State> Stored>
	inline void StateMachine<Stored>::remove(const std::string& key)
	{
		for (auto&& state : m_stack)
		{
			if (state->name() == key)
			{
				GALAXY_LOG(GALAXY_WARN, "Cant remove an active state.");
				return;
			}
		}

		const auto hash = math::fnv1a(key.c_str());
		m_storage.erase(hash);
	}

	template<std::derived_from<State> Stored>
	inline void StateMachine<Stored>::push(const std::string& key) noexcept
	{
		if (auto state = get(key))
		{
			m_stack.push_back(state);
			m_stack.back()->on_push();
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to push non-existent scene '{0}'.", key);
		}
	}

	template<std::derived_from<State> Stored>
	inline void StateMachine<Stored>::pop() noexcept
	{
		if (auto state = top())
		{
			state->on_pop();
			m_stack.pop_back();
		}

		if (m_stack.size() > 0)
		{
			m_stack.back()->on_pop();
		}
	}

	template<std::derived_from<State> Stored>
	inline void StateMachine<Stored>::pop_all() noexcept
	{
		while (!m_stack.empty())
		{
			pop();
		}
	}

	template<std::derived_from<State> Stored>
	inline std::shared_ptr<Stored> StateMachine<Stored>::top() const noexcept
	{
		if (!m_stack.empty())
		{
			return m_stack.back();
		}

		return nullptr;
	}

	template<std::derived_from<State> Stored>
	inline StateMachine<Stored>::Stack& StateMachine<Stored>::stack() noexcept
	{
		return m_stack;
	}

	template<std::derived_from<State> Stored>
	inline StateMachine<Stored>::Map& StateMachine<Stored>::storage() noexcept
	{
		return m_storage;
	}
} // namespace galaxy

#endif
