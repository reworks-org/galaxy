///
/// Cursor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Cursor.hpp"

namespace galaxy
{
	namespace input
	{
		Cursor::Cursor() noexcept
		    : m_data {nullptr}, m_pos {0.0, 0.0}, m_prev_pos {0.0, 0.0}, m_cursor_size {0.0, 0.0}
		{
		}

		Cursor::Cursor(Cursor&& c) noexcept
		{
			this->m_cursor_size = std::move(c.m_cursor_size);
			this->m_data        = c.m_data;
			this->m_pos         = std::move(c.m_pos);
			this->m_prev_pos    = std::move(c.m_prev_pos);

			c.m_data = nullptr;
		}

		Cursor& Cursor::operator=(Cursor&& c) noexcept
		{
			if (this != &c)
			{
				this->m_cursor_size = std::move(c.m_cursor_size);
				this->m_data        = c.m_data;
				this->m_pos         = std::move(c.m_pos);
				this->m_prev_pos    = std::move(c.m_prev_pos);

				c.m_data = nullptr;
			}

			return *this;
		}

		Cursor::~Cursor() noexcept
		{
			destroy();
		}

		void Cursor::destroy()
		{
			if (m_data != nullptr)
			{
				glfwDestroyCursor(m_data);
			}
		}
	} // namespace input
} // namespace galaxy