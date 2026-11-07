///
/// Panel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Panel.hpp"

namespace galaxy
{
	Panel::Panel() noexcept
		: m_show {false}
	{
	}

	Panel::Panel(Panel&& p) noexcept
	{
		this->m_show = p.m_show;
	}

	Panel::Panel(const Panel& p) noexcept
	{
		this->m_show = p.m_show;
	}

	Panel::~Panel() noexcept
	{
	}

	void Panel::toggle() noexcept
	{
		m_show = !m_show;
	}

	bool Panel::shown() const noexcept
	{
		return m_show;
	}

	Panel& Panel::operator=(Panel&& p) noexcept
	{
		if (this != &p)
		{
			this->m_show = p.m_show;
		}

		return *this;
	}

	Panel& Panel::operator=(const Panel& p) noexcept
	{
		if (this != &p)
		{
			this->m_show = p.m_show;
		}

		return *this;
	}
} // namespace galaxy
