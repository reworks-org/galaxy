///
/// Tooltip.hpp
/// starlight
///
/// Created by reworks on 12/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TOOLTIP_HPP_
#define STARLIGHT_TOOLTIP_HPP_

#include "sl/resources/FontBook.hpp"

namespace sl
{
	///
	/// Displays text when hovering over a widget.
	///
	class Tooltip final
	{
		friend class Widget;
	public:
		///
		/// Constructor.
		///
		/// \param text Text to display on the tooltip.
		/// \param font Font you want to use for the tooltip.
		/// \param col Colour of the tooltip.
		/// \param maxWidth Maximum width of each line of text of the tooltip, in characters.
		///
		Tooltip(const std::string& text, const std::string& font, const ALLEGRO_COLOR col, const float maxWidth);

		///
		/// \brief Destructor.
		///
		/// Clean up tooltip memory.
		///
		~Tooltip() noexcept = default;

		///
		/// \brief Allows for buttons to recieve mouse events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		void receive(const ALLEGRO_MOUSE_EVENT& e);

	private:
		///
		/// Draws the tooltip to the screen.
		///
		void draw();

	private:
		///
		/// x position of the tooltip.
		///
		float m_x;

		///
		/// y position of the tooltip.
		///
		float m_y;

		///
		/// Text to draw.
		///
		const char* m_text;
		
		///
		/// Max width for each line of tooltip text.
		///
		float m_maxWidth;

		///
		/// Line height of the font.
		///
		float m_lineHeight;

		///
		/// Font to draw tooltip in.
		///
		ALLEGRO_FONT* m_font;

		///
		/// Colour of the tooltip text.
		///
		ALLEGRO_COLOR m_col;
	};
}

#endif