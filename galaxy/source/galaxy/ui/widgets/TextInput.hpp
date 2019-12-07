///
/// TextInput.hpp
/// galaxy
///
/// Created by reworks on 24/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TEXTINPUT_HPP_
#define GALAXY_TEXTINPUT_HPP_

#include "galaxy/ui/Widget.hpp"
#include "galaxy/math/Vector4.hpp"
#include "galaxy/events/KeyCharEvent.hpp"
#include "galaxy/events/MousePressedEvent.hpp"

galaxy
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
		TextInput(const galaxyRect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, UITheme* theme);

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
		TextInput(const galaxyVector4<int>& vec4, const std::string& field, const std::string& indicator, UITheme* theme);

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
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void receiveMouseMoved(const galaxyMouseMovedEvent& e);

		///
		/// \brief Allows for the input field to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receiveMousePress(const galaxyMousePressedEvent& e);
		
		///
		/// \brief Allows for the input field to recieve unicode keyboard events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e KeyCharEvent object.
		///
		void receiveCharPress(const galaxyKeyCharEvent& e);

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

		///
		/// \brief Set the offset of the widget from the panel. Called for you in the Panel::add widget function.
		///
		/// It should look like this:
		/// m_bounds.m_x += x;
		/// m_bounds.m_y += y;
		///
		/// \param x x-pos of the panel.
		/// \param y y-pos of the panel.
		///
		void setOffset(const int x, const int y) override;

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