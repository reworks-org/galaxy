///
/// Mouse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Mouse.hpp"

namespace galaxy
{
	namespace input
	{
		Mouse::Mouse() noexcept
			: m_scroll_delta {0.0}
		{
			m_mouse_map.reserve(12);
			m_mouse_map.emplace(MouseButtons::BUTTON_1, GLFW_MOUSE_BUTTON_1);
			m_mouse_map.emplace(MouseButtons::BUTTON_2, GLFW_MOUSE_BUTTON_2);
			m_mouse_map.emplace(MouseButtons::BUTTON_3, GLFW_MOUSE_BUTTON_3);
			m_mouse_map.emplace(MouseButtons::BUTTON_4, GLFW_MOUSE_BUTTON_4);
			m_mouse_map.emplace(MouseButtons::BUTTON_5, GLFW_MOUSE_BUTTON_5);
			m_mouse_map.emplace(MouseButtons::BUTTON_6, GLFW_MOUSE_BUTTON_6);
			m_mouse_map.emplace(MouseButtons::BUTTON_7, GLFW_MOUSE_BUTTON_7);
			m_mouse_map.emplace(MouseButtons::BUTTON_8, GLFW_MOUSE_BUTTON_8);
			m_mouse_map.emplace(MouseButtons::BUTTON_LAST, GLFW_MOUSE_BUTTON_LAST);
			m_mouse_map.emplace(MouseButtons::BUTTON_LEFT, GLFW_MOUSE_BUTTON_LEFT);
			m_mouse_map.emplace(MouseButtons::BUTTON_RIGHT, GLFW_MOUSE_BUTTON_RIGHT);
			m_mouse_map.emplace(MouseButtons::BUTTON_MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE);

			m_reverse_mouse_map.reserve(12);
			for (const auto& [key, value] : m_mouse_map)
			{
				m_reverse_mouse_map.emplace(value, key);
			}

			m_prev_mouse_btn_states.fill(GLFW_RELEASE);
		}

		Mouse::Mouse(Mouse&& m) noexcept
		{
			this->m_mouse_map             = std::move(m.m_mouse_map);
			this->m_prev_mouse_btn_states = std::move(m.m_prev_mouse_btn_states);
			this->m_reverse_mouse_map     = std::move(m.m_reverse_mouse_map);
			this->m_scroll_delta          = m.m_scroll_delta;
		}

		Mouse& Mouse::operator=(Mouse&& m) noexcept
		{
			if (this != &m)
			{
				this->m_mouse_map             = std::move(m.m_mouse_map);
				this->m_prev_mouse_btn_states = std::move(m.m_prev_mouse_btn_states);
				this->m_reverse_mouse_map     = std::move(m.m_reverse_mouse_map);
				this->m_scroll_delta          = m.m_scroll_delta;
			}

			return *this;
		}

		Mouse::Mouse(const Mouse& m) noexcept
		{
			this->m_mouse_map             = m.m_mouse_map;
			this->m_prev_mouse_btn_states = m.m_prev_mouse_btn_states;
			this->m_reverse_mouse_map     = m.m_reverse_mouse_map;
			this->m_scroll_delta          = m.m_scroll_delta;
		}

		Mouse& Mouse::operator=(const Mouse& m) noexcept
		{
			if (this != &m)
			{
				this->m_mouse_map             = m.m_mouse_map;
				this->m_prev_mouse_btn_states = m.m_prev_mouse_btn_states;
				this->m_reverse_mouse_map     = m.m_reverse_mouse_map;
				this->m_scroll_delta          = m.m_scroll_delta;
			}

			return *this;
		}

		Mouse::~Mouse() noexcept
		{
			m_mouse_map.clear();
			m_reverse_mouse_map.clear();
		}
	} // namespace input
} // namespace galaxy