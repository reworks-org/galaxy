///
/// Tooltip.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TOOLTIP_HPP_
#define CELESTIAL_TOOLTIP_HPP_

#include <protostar/events/MouseMovedEvent.hpp>

#include "celestial/ui/UITheme.hpp"
#include "celestial/interface/Renderer.hpp"

namespace celestial
{
	///
	/// Displays text when hovering over a widget.
	///
	class Tooltip final
	{
		///
		/// Allows access to internals of Widget class.
		///
		friend class Widget;

	public:
		///
		/// Texture Constructor.
		///
		/// \param text Text to display on the tooltip.
		/// \param texture Texture of the tooltip. Will repeat to create variable sized textboxes.
		///                Format should be Open Of Tooltip, Repeat Section For Text, Closing for tooltip, all seemless so it forms a complete graphic.
		///				   I.e. your string should look like: "tooltip_widget|1|97", with x being x left most and 97 being rightmost x.
		/// \param theme Theme the tooltip is using for the master texture. Also font and text colour.
		///
		Tooltip(const std::string& text, const std::string& texture, UITheme* theme);

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
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		void receive(const protostar::MouseMovedEvent& e);

		///
		/// Draws the tooltip to the screen.
		///
		void draw(celestial::Renderer* renderer);

	private:
		///
		/// Default constructor.
		///
		Tooltip() = delete;

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
		std::string m_text;

		///
		/// Texture background.
		///
		TexturePtr m_texture;

		///
		/// Theme the tooltip is using for font and colour.
		///
		UITheme* m_theme;
	};
}

#endif