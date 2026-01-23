///
/// Event.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Event.hpp"

namespace galaxy
{
	Event::Event() noexcept
		: m_handled {false}
	{
	}

	Event::Event(Event&& e) noexcept
	{
		this->m_handled = e.m_handled;
	}

	Event& Event::operator=(Event&& e) noexcept
	{
		if (this != &e)
		{
			this->m_handled = e.m_handled;
		}

		return *this;
	}

	Event::Event(const Event& e) noexcept
	{
		this->m_handled = e.m_handled;
	}

	Event& Event::operator=(const Event& e) noexcept
	{
		if (this != &e)
		{
			this->m_handled = e.m_handled;
		}

		return *this;
	}

	Event::~Event() noexcept
	{
	}

	void Event::consume() noexcept
	{
		m_handled = true;
	}

	bool Event::consumed() const noexcept
	{
		return m_handled;
	}
} // namespace galaxy
