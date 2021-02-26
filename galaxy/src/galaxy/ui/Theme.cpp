///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Theme.hpp"

namespace galaxy
{
	namespace ui
	{
		Theme::Theme() noexcept
		    : m_font_col {255, 0, 0, 255}
		{
		}

		Theme::Theme(Theme&& t) noexcept
		{
			this->m_font_col = std::move(t.m_font_col);
			this->m_camera   = std::move(t.m_camera);
		}

		Theme& Theme::operator=(Theme&& t) noexcept
		{
			if (this != &t)
			{
				this->m_font_col = std::move(t.m_font_col);
				this->m_camera   = std::move(t.m_camera);
			}

			return *this;
		}
	} // namespace ui
} // namespace galaxy