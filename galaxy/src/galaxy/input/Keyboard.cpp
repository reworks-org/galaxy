///
/// Keyboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Keyboard.hpp"

namespace galaxy
{
	Keyboard::Keyboard() noexcept
	{
	}

	Keyboard::~Keyboard() noexcept
	{
	}

	void Keyboard::begin_text_input(const SDL_TextInputType input_type, const SDL_Capitalization capitals, const bool multiline) const noexcept
	{
		if (!SDL_TextInputActive(m_window))
		{
			const auto props = SDL_CreateProperties();
			SDL_SetNumberProperty(props, SDL_PROP_TEXTINPUT_TYPE_NUMBER, input_type);
			SDL_SetNumberProperty(props, SDL_PROP_TEXTINPUT_CAPITALIZATION_NUMBER, capitals);
			SDL_SetBooleanProperty(props, SDL_PROP_TEXTINPUT_AUTOCORRECT_BOOLEAN, false);
			SDL_SetBooleanProperty(props, SDL_PROP_TEXTINPUT_MULTILINE_BOOLEAN, multiline);

			SDL_StartTextInputWithProperties(m_window, props);
			SDL_DestroyProperties(props);
		}
	}

	void Keyboard::end_text_input() const noexcept
	{
		SDL_StopTextInput(m_window);
	}

	void Keyboard::clear_state() const noexcept
	{
		SDL_ResetKeyboard();
	}

	bool Keyboard::has_keyboard() const noexcept
	{
		return SDL_HasKeyboard();
	}

	bool Keyboard::has_onscreen_keyboard() const noexcept
	{
		return SDL_HasScreenKeyboardSupport();
	}

	bool Keyboard::onscreen_keyboard_active() const noexcept
	{
		return SDL_ScreenKeyboardShown(m_window);
	}
} // namespace galaxy
