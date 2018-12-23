///
/// Slider.hpp
/// starlight
///
/// Created by reworks on 17/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SLIDER_HPP_
#define STARLIGHT_SLIDER_HPP_

#include "sl/ui/Widget.hpp"
#include "sl/events/MousePressedEvent.hpp"

namespace sl
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
		/// \param markerW Width of the marker object.
		/// \param markerH Height of the marker object.
		/// \param slider Colour of the slider itself.
		/// \param marker Colour of the marker used to indicate position.
		///
		Slider(const sl::Rect<int>& bounds, const int markerW, const int markerH, const ALLEGRO_COLOR slider, const ALLEGRO_COLOR marker);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		///
		Slider(const sol::table& table);

		///
		/// Image constructor.
		///
		/// \param x x-pos relative to panel.
		/// \param y y-pos relative to panel.
		/// \param slider Texture of the slider.
		/// \param marker Texture of the marker.
		///
		Slider(const int x, const int y, const std::string& slider, const std::string& marker);

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
		void receiveMove(const sl::MouseMovedEvent& e);

		///
		/// \brief Allows for Slider to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receivePress(const sl::MousePressedEvent& e);

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
		/// Get current value of slider.
		///
		/// \return const float of value. 0.0f - 1.0f. I.e. 0.78f is 78%.
		///
		const float getValue() const;

		///
		/// Returns current progress of bar in a percentage.
		///
		/// \return const float progress as pertentage.
		///
		const float getPercentage() const;

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
		/// Slider texture.
		///
		ALLEGRO_BITMAP* m_slider;

		///
		/// Marker texture.
		///
		ALLEGRO_BITMAP* m_marker;
	};
}

#endif