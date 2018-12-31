///
/// TextInput.hpp
/// starlight
///
/// Created by reworks on 24/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TEXTINPUT_HPP_
#define STARLIGHT_TEXTINPUT_HPP_

#include "sl/ui/Widget.hpp"
#include "sl/math/Vector4.hpp"
#include "sl/events/KeyCharEvent.hpp"
#include "sl/events/MousePressedEvent.hpp"

namespace sl
{
	///
	/// Allows you to input text through a UI widget.
	///
	class TextInput : public Widget
	{
	public:
		///
		/// \brief Primitives constructor.
		///
		///  Text is drawn centered to field.
		///
		/// \param bounds x, y, w, h of field to draw, relative to the panel.
		/// \param field Colour of field to draw.
		/// \param indicator Colour of indicator to draw.
		/// \param theme Theme the widget will use.
		///
		TextInput(const sl::Rect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, UITheme* theme);

		///
		/// Texture constructor.
		///
		/// \param vec4 w = x-pos of field relative to panel.
		///				x = y-pos of field relative to panel.
		///				y = x-pos of text to draw relative to field.
		///				z = y-pos of text to draw relative to field.
		/// \param field Texture of field to in theme to use.
		/// \param indicator Texture of text position indicator in theme to use.
		/// \param theme Theme the widget will use.
		///
		TextInput(const sl::Vector4<int>& vec4, const std::string& field, const std::string& indicator, UITheme* theme);

		///
		/// Lua constructor.
		///
		/// \param table Lua table containing data to construct widget from.
		/// \param theme Theme the widget will use.
		///
		TextInput(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~TextInput() noexcept;

		///
		/// \brief Allows for input field to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void receiveMouseMoved(const sl::MouseMovedEvent& e);

		///
		/// \brief Allows for the input field to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receiveMousePress(const sl::MousePressedEvent& e);
		
		///
		/// \brief Allows for the input field to recieve unicode keyboard events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e KeyCharEvent object.
		///
		void receiveCharPress(const sl::KeyCharEvent& e);

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		void render() override;

		///
		/// Stops input to the widget.
		///
		void stopInput();

		///
		/// Get current widget text.
		///
		/// \return Returns the current inputted text in a const char*.
		///
		std::string getText();

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		TextInput() = delete;

	private:
		///
		/// Is the indicator drawable.
		///
		bool m_drawIndicator;

		///
		/// Time passed for indicator.
		///
		double m_timePassed;

		///
		/// Keeps track to see if the input form is selected or not.
		///
		bool m_isSelected;
		
		///
		/// Position of cursor in text.
		///
		int m_cursorPos;

		///
		/// Position to draw the text from.
		///
		int m_startPos;

		///
		/// Text that was inputted.
		///
		ALLEGRO_USTR* m_text;

		///
		/// Texture for input field.
		///
		ALLEGRO_BITMAP* m_field;

		///
		/// Texture for current character position indicator.
		///
		ALLEGRO_BITMAP* m_indicator;

		///
		/// x-pos to draw text at on the widget, relative to the field texture x-pos.
		/// 
		int m_textX;

		///
		/// y-pos to draw text at on the widget, relative to the field texture y-pos.
		///
		int m_textY;

		///
		/// Used to store info from al_ref functions.
		///
		ALLEGRO_USTR_INFO m_info;
	};
}

#endif