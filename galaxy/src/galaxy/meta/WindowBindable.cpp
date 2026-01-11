///
/// WindowBindable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/core/Window.hpp"

#include "WindowBindable.hpp"

namespace galaxy
{
	WindowBindable::WindowBindable() noexcept
		: m_window {nullptr}
	{
		bind_window();
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

	WindowBindable::~WindowBindable() noexcept
	{
		m_window = nullptr;
	}

	void WindowBindable::bind_window() noexcept
	{
		auto& window = entt::locator<Window>::value();
		m_window     = window.handle();
	}
} // namespace galaxy
