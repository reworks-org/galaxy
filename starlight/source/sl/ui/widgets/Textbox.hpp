///
/// Textbox.hpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TEXTBOX_HPP_
#define STARLIGHT_TEXTBOX_HPP_

#include "sl/ui/Widget.hpp"
#include "sl/events/KeyDownEvent.hpp"

namespace sl
{
	///
	/// Represents a message box or "textbox" in the style of a JRPG.
	///
	class Textbox : public Widget
	{
	public:
		///
		/// Constructor.
		///
		///
		/// \param x x-pos relative to panel.
		/// \param y y-pos relative to panel.
		/// \param frame The texture for the textbox frame.
		/// \param indicator The texture that indicates when this message is finished.
		/// \param messages List of messages to be displayed. Each entry in the vector is a message for one "page" of the text box.
		/// \param font The font to draw the text in.
		/// \param maxWidth Maximum width for each line of the text in the textbox. In pixels.
		/// \param col Colour of the text to draw.
		/// \param duration The duration in which to display each character at. 0 to always display text at once. In milliseconds.
		/// \param speaker Optional. Specifys if a "speaker" is needed for the textbox. I.e. who is talking right now.
		///
		Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const std::string& font, const unsigned int maxWidth, const ALLEGRO_COLOR col, const unsigned int duration, const std::string& speaker = "");

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		///
		Textbox(const sol::table& table);

		///
		/// Destructor.
		///
		~Textbox() noexcept;

		///
		/// \brief Allows for the textbox to recieve keyboard events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e KeyDownEvent object.
		///
		void receive(const sl::KeyDownEvent& e);

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
		Textbox() = delete;

	private:
		///
		/// Frame texture.
		///
		ALLEGRO_BITMAP* m_frame;

		///
		/// Indicator texture.
		///
		ALLEGRO_BITMAP* m_indicator;

		///
		/// Messages to display with this text box. Each entry in the vector is another "page" for the textbox.
		///
		std::vector<std::string> m_messages;

		///
		/// Current speaker.
		///
		std::string m_speaker;

		///
		/// Font to render text with.
		///
		ALLEGRO_FONT* m_font;
	
		///
		/// Maximum length for each line of the text in the textbox.
		///
		unsigned int m_maxWidth;

		///
		/// Colour of the text.
		///
		ALLEGRO_COLOR m_colour;

		///
		/// Duration to display characters at. 0 to disable. In milliseconds.
		///
		unsigned int m_duration;

		///
		/// Amount of time passed when counting to see if next character is to be drawn.
		///
		double m_timePassed;

		///
		/// Time passed for indicator.
		///
		double m_indicatorTimePassed;

		///
		/// Current message string index.
		///
		std::size_t m_characterIndex;

		///
		/// Current "page" of textbox.
		///
		std::size_t m_page;

		///
		/// Toggle to draw lowered indicator or not.
		///
		bool m_drawLoweredIndicator;

		///
		/// Line height of the font.
		///
		int m_lineHeight;
	};
}

#endif