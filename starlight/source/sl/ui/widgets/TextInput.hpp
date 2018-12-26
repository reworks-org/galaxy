



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
#include "sl/events/KeyDownEvent.hpp"
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
		/// Primitives constructor.
		///
		TextInput(const sl::Rect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, const std::string& font, const ALLEGRO_COLOR textCol);

		///
		/// Texture constructor.
		///
		TextInput(const int x, const int y, const std::string& field, const std::string& indicator, const std::string& font, const ALLEGRO_COLOR textCol);

		///
		/// Lua constructor.
		///
		TextInput(const sol::table& table);

		///
		/// Destructor.
		///
		~TextInput() noexcept;

		///
		/// \brief Allows for the input field to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receiveMousePress(const sl::MousePressedEvent& e);
		
		///
		/// \brief Allows for the input field to recieve keyboard events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e KeyDownEvent object.
		///
		void receiveKeyPress(const sl::KeyDownEvent& e);

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

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		TextInput() = delete;

	private:
		///
		/// Text that was inputted.
		///
		std::string m_text;

		///
		/// Keeps track to see if the input form is selected or not.
		///
		bool m_isSelected;

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
	};
}