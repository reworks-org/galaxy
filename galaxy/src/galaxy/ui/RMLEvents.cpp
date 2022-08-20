///
/// RMLEvents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <RmlUi/Core.h>

#include "galaxy/ui/RMLInput.hpp"

#include "RMLEvents.hpp"

namespace galaxy
{
	namespace ui
	{
		RMLEvents::RMLEvents() noexcept
			: m_context {nullptr}
		{
		}

		RMLEvents::~RMLEvents() noexcept
		{
			m_context = nullptr;
		}

		void RMLEvents::set_context(Rml::Context* context) noexcept
		{
			m_context = context;
		}

		void RMLEvents::on_mouse_move(events::MouseMoved& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessMouseMove(static_cast<int>(std::trunc(e.m_xpos)), static_cast<int>(std::trunc(e.m_ypos)), 0);
			}
		}

		void RMLEvents::on_mouse_pressed(events::MousePressed& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessMouseButtonDown(static_cast<int>(e.m_button), 0);
			}
		}

		void RMLEvents::on_mouse_released(events::MouseReleased& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessMouseButtonUp(static_cast<int>(e.m_button), 0);
			}
		}

		void RMLEvents::on_mouse_wheel(events::MouseWheel& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessMouseWheel(static_cast<float>(e.m_yoff), 0);
			}
		}

		void RMLEvents::on_key_down(events::KeyDown& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessKeyDown(ui::RMLInput::handle().m_keymap[e.m_keycode], ui::RMLInput::handle().m_modmap[e.m_mod]);
			}
		}

		void RMLEvents::on_key_up(events::KeyUp& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				e.m_handled = !m_context->ProcessKeyUp(ui::RMLInput::handle().m_keymap[e.m_keycode], ui::RMLInput::handle().m_modmap[e.m_mod]);
			}
		}

		void RMLEvents::on_key_char(events::KeyChar& e) noexcept
		{
			if (!e.m_handled && (m_context != nullptr))
			{
				if (const Rml::Element* element = m_context->GetFocusElement())
				{
					const auto& tag = element->GetTagName();
					if (tag == "input" || tag == "textarea" || tag == "select")
					{
						e.m_handled = m_context->ProcessTextInput(e.m_char);
					}
				}
			}
		}

		void RMLEvents::on_window_resize(const events::WindowResized& e) noexcept
		{
			if (m_context != nullptr)
			{
				m_context->SetDimensions({e.m_width, e.m_height});
			}
		}
	} // namespace ui
} // namespace galaxy