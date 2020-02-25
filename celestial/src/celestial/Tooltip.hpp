///
/// Tooltip.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TOOLTIP_HPP_
#define CELESTIAL_TOOLTIP_HPP_

#include <protostar/events/MouseMovedEvent.hpp>

#include "celestial/UITheme.hpp"
#include "celestial/interface/Texture.hpp"
#include "celestial/interface/Renderer.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Tooltip for widgets.
	///
	class Tooltip final
	{
	public:
		///
		/// Constructor.
		///
		Tooltip() noexcept;

		///
		/// Destructor.
		///
		~Tooltip() noexcept;

		///
		/// Allows for buttons to react to mouse events.
		///
		/// \param e A MouseMovedEvent object to check if TextureTooltip is active.
		///
		void onMove(const protostar::MouseMovedEvent& e) noexcept;

		///
		/// Draws the TextureTooltip to the screen.
		///
		/// \param renderer Renderer to use when drawing.
		///
		void draw(celestial::interface::Renderer* renderer) noexcept;

		///
		/// Set UITheme.
		///
		/// \param theme Pointer to theme to use.
		///
		void setTheme(celestial::UITheme* theme) noexcept;

		///
		/// Set text.
		///
		/// \param text Text to display on tooltip.
		///
		void setText(const std::string& text) noexcept;

		///
		/// Set texture.
		///
		/// \param texture Texture of the tooltip. Will repeat to create variable sized textboxes.
		///                Format should be Open Of Tooltip, Repeat Section For Text, Closing for tooltip, all seemless so it forms a complete graphic.
		///				   I.e. your string should look like: "tooltip_widget|1|97", with x being x left most and 97 being rightmost x.
		///
		void setTexture(const std::string& texture) noexcept;

	protected:
		///
		/// x position of the TextureTooltip.
		///
		double m_x;

		///
		/// y position of the TextureTooltip.
		///
		double m_y;

		///
		/// Text to draw.
		///
		std::string m_text;

		///
		/// Theme the TextureTooltip is using for font and colour.
		///
		UITheme* m_theme;

		///
		/// Texture extracted from UITheme.
		///
		celestial::interface::TexturePtr m_texture;
	};
}

#endif