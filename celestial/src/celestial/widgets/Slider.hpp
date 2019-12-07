///
/// Slider.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_SLIDER_HPP_
#define CELESTIAL_SLIDER_HPP_

#include "celestial/ui/Widget.hpp"
#include "celestial/events/MousePressedEvent.hpp"

namespace celestial
{
	///
	/// UI bar that can be moved to increment and decrement a value.
	///
	class Slider final : public Widget
	{
	public:
		///
		/// Primitives constructor.
		///
		/// \param bounds x,y,w,h of slider rectangle.
		/// \param mw Width of the marker object.
		/// \param mh Height of the marker object.
		/// \param slider Colour of the slider itself.
		/// \param marker Colour of the marker used to indicate position.
		/// \param loader ResourceLoader to use.
		///
		Slider(const celestial::Rect<int>& bounds, const int mw, const int mh, const celestial::compat::Colour* slider, const celestial::compat::Colour* marker, celestial::ResourceLoader* loader);

		///
		/// Texture constructor.
		///
		/// \param x x-pos of widget relative to panel.
		/// \param y y-pos of widget relative to panel.
		/// \param slider Texture of the slider in theme to use.
		/// \param marker Texture of the marker in theme to use.
		/// \param theme Theme of the widget to use.
		///
		Slider(const int x, const int y, const std::string& slider, const std::string& marker, UITheme* theme);

		///
		/// Destructor.
		///
		~Slider() noexcept;

		///
		/// \brief Allows for Slider to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void receiveMove(const celestial::MouseMovedEvent& e);

		///
		/// \brief Allows for Slider to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receivePress(const celestial::MousePressedEvent& e);

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		void render(celestial::compat::Renderer* renderer) override;

		///
		/// Get current value of slider.
		///
		/// \return const int of value. 0.0f - 1.0f. I.e. 0.78f is 78%.
		///
		const float getValue() const;

		///
		/// Returns current progress of bar in a percentage.
		///
		/// \return const int progress as pertentage.
		///
		const float getPercentage() const;

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
		Slider() = delete;

	private:
		///
		/// Current value of this bar. 0.0f - 1.0f. I.e. 0.78f is 78%.
		///
		float m_value;

		///
		/// X position of marker on bar.
		///
		float m_markerX;

		///
		/// Marker width.
		///
		int m_markerW;

		///
		/// Marker height.
		///
		int m_markerH;

		///
		/// Slider texture.
		///
		TexturePtr m_slider;

		///
		/// Marker texture.
		///
		TexturePtr m_marker;
	};
}

#endif