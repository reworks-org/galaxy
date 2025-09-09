///
/// WindowBindable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "WindowBindable.hpp"

namespace galaxy
{
	WindowBindable::~WindowBindable() noexcept
	{
		m_window = nullptr;
	}

	WindowBindable::WindowBindable() noexcept
		: m_window {nullptr}
	{
	}

	WindowBindable::WindowBindable(WindowBindable&& w) noexcept
	{
		this->m_window = w.m_window;
		w.m_window     = nullptr;
	}

	WindowBindable& WindowBindable::operator=(WindowBindable&& w) noexcept
	{
		if (this != &w)
		{
			this->m_window = w.m_window;
			w.m_window     = nullptr;
		}

		return *this;
	}

	WindowBindable::WindowBindable(const WindowBindable& w) noexcept
	{
		this->m_window = w.m_window;
	}

	WindowBindable& WindowBindable::operator=(const WindowBindable& w) noexcept
	{
		if (this != &w)
		{
			this->m_window = w.m_window;
		}

		return *this;
	}

	void WindowBindable::bind_window(SDL_Window* window) noexcept
	{
		m_window = window;
	}
} // namespace galaxy
