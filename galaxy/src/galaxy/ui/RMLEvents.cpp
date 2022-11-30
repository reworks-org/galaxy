///
/// RMLEvents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>
#include <RmlUi/Core.h>

#include "RMLEvents.hpp"

namespace galaxy
{
	namespace ui
	{
		RMLEvents::RMLEvents(Rml::Context* context) noexcept
			: m_context {context}
		{
		}

		RMLEvents::~RMLEvents() noexcept
		{
			m_context = nullptr;
		}

		void RMLEvents::on_mouse_move(events::MouseMoved& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessMouseMove(static_cast<int>(std::trunc(e.m_xpos)), static_cast<int>(std::trunc(e.m_ypos)), 0);
			}
		}

		void RMLEvents::on_mouse_pressed(events::MousePressed& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessMouseButtonDown(static_cast<int>(e.m_button), convert_key_modifier(static_cast<int>(e.m_mod)));
			}
		}

		void RMLEvents::on_mouse_released(events::MouseReleased& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessMouseButtonUp(static_cast<int>(e.m_button), convert_key_modifier(static_cast<int>(e.m_mod)));
			}
		}

		void RMLEvents::on_mouse_wheel(events::MouseWheel& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessMouseWheel(static_cast<float>(e.m_yoff), 0);
			}
		}

		void RMLEvents::on_key_down(events::KeyDown& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessKeyDown(RMLInput::handle().m_keymap[e.m_keycode], RMLInput::handle().m_modmap[e.m_mod]);
			}
		}

		void RMLEvents::on_key_repeat(events::KeyRepeat& e) noexcept
		{
			e.m_handled = !m_context->ProcessKeyDown(RMLInput::handle().m_keymap[e.m_keycode], RMLInput::handle().m_modmap[e.m_mod]);
			if (e.m_keycode == input::Keys::ENTER || e.m_keycode == input::Keys::KP_ENTER && e.m_handled)
			{
				m_context->ProcessTextInput('\n');
			}
		}

		void RMLEvents::on_key_up(events::KeyUp& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessKeyUp(RMLInput::handle().m_keymap[e.m_keycode], RMLInput::handle().m_modmap[e.m_mod]);
			}
		}

		void RMLEvents::on_key_char(events::KeyChar& e) noexcept
		{
			if (!e.m_handled)
			{
				e.m_handled = !m_context->ProcessTextInput(static_cast<Rml::Character>(e.m_codepoint));
			}
		}

		void RMLEvents::on_window_resize(const events::WindowResized& e) noexcept
		{
			m_context->SetDimensions({e.m_width, e.m_height});
		}

		int RMLEvents::convert_key_modifier(int glfw_mods) noexcept
		{
			int key_modifier_state = 0;

			if (GLFW_MOD_SHIFT & glfw_mods)
			{
				key_modifier_state |= Rml::Input::KM_SHIFT;
			}

			if (GLFW_MOD_CONTROL & glfw_mods)
			{
				key_modifier_state |= Rml::Input::KM_CTRL;
			}

			if (GLFW_MOD_ALT & glfw_mods)
			{
				key_modifier_state |= Rml::Input::KM_ALT;
			}

			if (GLFW_MOD_CAPS_LOCK & glfw_mods)
			{
				key_modifier_state |= Rml::Input::KM_SCROLLLOCK;
			}

			if (GLFW_MOD_NUM_LOCK & glfw_mods)
			{
				key_modifier_state |= Rml::Input::KM_NUMLOCK;
			}

			return key_modifier_state;
		}
	} // namespace ui
} // namespace galaxy