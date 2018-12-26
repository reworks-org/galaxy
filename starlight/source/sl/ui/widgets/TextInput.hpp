



// recieve a button press, toggle on / off input.
// allows for input to be recieved then append each key press to string and draw it. 
// probs need a texture version and a primitives version?
// and a lua constructor for texture version



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
		/// \param font Font to draw text in.
		/// \param textCol Colour to draw text in.
		///
		TextInput(const sl::Rect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, const std::string& font, const ALLEGRO_COLOR textCol);

		///
		/// Texture constructor.
		///
		/// \param x x-pos of field to draw, relative to the panel.
		/// \param y y-pos of field to draw, relative to the panel.
		/// \param field Texture of field to load.
		/// \param indicator Texture of text position indicator to load.
		/// \param font Font to draw text in.
		/// \param textCol Colour to draw text in.
		/// \param textX x-pos to draw text at on the widget, relative to the field texture x-pos.
		/// \param textY y-pos to draw text at on the widget, relative to the field texture y-pos.
		///
		TextInput(const int x, const int y, const std::string& field, const std::string& indicator, const std::string& font, const ALLEGRO_COLOR textCol, int textX, int textY);

		///
		/// Lua constructor.
		///
		/// \param table Lua table containing data to construct widget from.
		///
		TextInput(const sol::table& table);

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
		/// Font used for widget.
		///
		ALLEGRO_FONT* m_font;

		///
		/// Colour for text.
		///
		ALLEGRO_COLOR m_colour;

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