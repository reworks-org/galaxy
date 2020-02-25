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
#include "celestial/interface/Renderer.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Tooltip parent class.
	///
	class Tooltip
	{
	public:
		///
		/// Default virtual destructor.
		///
		virtual ~Tooltip() noexcept = default;

		///
		/// Allows for buttons to react to mouse events.
		///
		/// \param e A MouseMovedEvent object to check if TextureTooltip is active.
		///
		virtual void onMove(const protostar::MouseMovedEvent& e) noexcept = 0;

		///
		/// Draws the TextureTooltip to the screen.
		///
		/// \param renderer Renderer to use when drawing.
		///
		virtual void draw(celestial::interface::Renderer* renderer) = 0;

		///
		/// Set UITheme.
		///
		/// \param theme Pointer to theme to use.
		///
		virtual void setTheme(celestial::UITheme* theme) noexcept final;

		///
		/// Set text.
		///
		/// \param text Text to display on tooltip.
		///
		virtual void setText(const std::string& text) noexcept final;

	protected:
		///
		/// Constructor.
		///
		Tooltip() noexcept = default;

		///
		/// Copy constructor.
		///
		Tooltip(const Tooltip&) noexcept = default;

		///
		/// Move constructor.
		///
		Tooltip(Tooltip&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		Tooltip& operator= (const Tooltip&&) noexcept = default;

		///
		/// Move assignment operator.
		///
		Tooltip& operator= (Tooltip&&) noexcept = default;

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
	};
}

#endif