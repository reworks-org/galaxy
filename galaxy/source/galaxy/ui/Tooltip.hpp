///
/// Tooltip.hpp
/// galaxy
///
/// Created by reworks on 12/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOOLTIP_HPP_
#define GALAXY_TOOLTIP_HPP_

#include "galaxy/math/Vector2.hpp"
#include "galaxy/resources/FontBook.hpp"
#include "galaxy/events/MouseMovedEvent.hpp"


namespace galaxy
{
	class UITheme;
	
	///
	/// Displays text when hovering over a widget.
	///
	class Tooltip final
	{
		friend class Widget;
	public:
		///
		/// Texture Constructor.
		///
		/// \param text Text to display on the tooltip.
		/// \param texture Texture of the tooltip. Will be repeated across tooltip box from section.x to section.y.
		///                Format should be Open Of Tooltip, Repeat Section For Text, Closing for tooltip, all seemless so it forms a complete graphic.
		/// \param section The part of the texture to repeat to create variable sized textboxes. x is left coord and y is right coord across width not height!
		/// \param theme Theme the tooltip is using for the master texture. Also font and text colour.
		///
		Tooltip(const std::string& text, const std::string& texture, const galaxy::Vector2<int>& section, UITheme* theme);

		///
		/// Primitives Constructor.
		///
		/// \param text Text to display on the tooltip.
		/// \param theme Theme the tooltip is using for font and colour.
		///
		Tooltip(const std::string& text, UITheme* theme);

		///
		/// \brief Destructor.
		///
		/// Clean up tooltip memory.
		///
		~Tooltip() noexcept;

		///
		/// \brief Allows for buttons to recieve mouse events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxy::Locator::dispatcher).
		///
		void receive(const galaxy::MouseMovedEvent& e);

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
		ALLEGRO_BITMAP* m_text;

		///
		/// Theme the tooltip is using for font and colour.
		///
		UITheme* m_theme;
	};
}

#endif