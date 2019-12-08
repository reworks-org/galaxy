///
/// Textbox.hpp
/// galaxy
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TEXTBOX_HPP_
#define GALAXY_TEXTBOX_HPP_

#include "galaxy/ui/Widget.hpp"
#include "galaxy/events/KeyDownEvent.hpp"

namespace galaxy
{
	///
	/// Represents a message box or "textbox" in the style of a JRPG.
	///
	class Textbox : public Widget
	{
	public:
		///
		/// Texture Constructor.
		///
		///
		/// \param x x-pos relative to panel.
		/// \param y y-pos relative to panel.
		/// \param frame The texture for the textbox frame in theme to use.
		/// \param indicator The texture that indicates when this message is finished in theme to use.
		/// \param messages List of messages to be displayed. Each entry in the vector is a message for one "page" of the text box.
		/// \param maxWidth Maximum width for each line of the text in the textbox. In pixels.
		/// \param duration The duration in which to display each character at. 0 to always display text at once. In milliseconds.
		/// \param theme Theme the widget will use.
		/// \param speaker Optional. Specifys if a "speaker" is needed for the textbox. I.e. who is talking right now.
		///
		Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const unsigned int maxWidth, const unsigned int duration, UITheme* theme, const std::string& speaker = "");

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme the widget will use.
		///
		Textbox(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~Textbox() noexcept;

		///
		/// \brief Allows for the textbox to recieve keyboard events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxy::Locator::dispatcher).
		///
		/// \param e KeyDownEvent object.
		///
		void receive(const galaxy::KeyDownEvent& e);

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
		/// Maximum length for each line of the text in the textbox.
		///
		unsigned int m_maxWidth;

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