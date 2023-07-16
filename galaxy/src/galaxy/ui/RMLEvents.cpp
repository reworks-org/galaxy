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
		RMLEvents::RMLEvents(Rml::Context* context)
			: m_context {context}
		{
		}

		RMLEvents::~RMLEvents()
		{
			m_context = nullptr;
		}

		void RMLEvents::on_mouse_move(events::MouseMoved& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessMouseMove(static_cast<int>(std::trunc(e.xpos)), static_cast<int>(std::trunc(e.ypos)), 0);
			}
		}

		void RMLEvents::on_mouse_pressed(events::MousePressed& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessMouseButtonDown(static_cast<int>(e.button), convert_key_modifier(static_cast<int>(e.mod)));
			}
		}

		void RMLEvents::on_mouse_released(events::MouseReleased& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessMouseButtonUp(static_cast<int>(e.button), convert_key_modifier(static_cast<int>(e.mod)));
			}
		}

		void RMLEvents::on_mouse_wheel(events::MouseWheel& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessMouseWheel(static_cast<float>(e.yoff), 0);
			}
		}

		void RMLEvents::on_mouse_enter(events::MouseEnter& e)
		{
			if (!e.handled && !e.entered)
			{
				e.handled = !m_context->ProcessMouseLeave();
			}
		}

		void RMLEvents::on_key_down(events::KeyDown& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessKeyDown(RMLInput::handle().m_keymap[e.keycode], RMLInput::handle().m_modmap[e.mod]);
			}
		}

		void RMLEvents::on_key_repeat(events::KeyRepeat& e)
		{
			e.handled = !m_context->ProcessKeyDown(RMLInput::handle().m_keymap[e.keycode], RMLInput::handle().m_modmap[e.mod]);
			if (e.keycode == input::Keys::ENTER || e.keycode == input::Keys::KP_ENTER && e.handled)
			{
				m_context->ProcessTextInput('\n');
			}
		}

		void RMLEvents::on_key_up(events::KeyUp& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessKeyUp(RMLInput::handle().m_keymap[e.keycode], RMLInput::handle().m_modmap[e.mod]);
			}
		}

		void RMLEvents::on_key_char(events::KeyChar& e)
		{
			if (!e.handled)
			{
				e.handled = !m_context->ProcessTextInput(static_cast<Rml::Character>(e.codepoint));
			}
		}

		void RMLEvents::on_window_resize(const events::WindowResized& e)
		{
			m_context->SetDimensions({e.width, e.height});
		}

		void RMLEvents::on_content_scale(const events::ContentScale& e)
		{
			m_context->SetDensityIndependentPixelRatio(e.xscale);
		}

		int RMLEvents::convert_key_modifier(int glfw_mods)
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