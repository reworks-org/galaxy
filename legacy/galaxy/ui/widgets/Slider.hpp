///
/// Slider.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_SLIDER_HPP_
#define GALAXY_UI_WIDGETS_SLIDER_HPP_

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// UI bar that can be moved to increment and decrement a value.
		///
		class Slider final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Slider() noexcept;

			///
			/// Destructor.
			///
			virtual ~Slider() noexcept = default;

			///
			/// Create slider.
			///
			/// \param slider Texture of the slider in theme to use.
			/// \param marker Texture of the marker in theme to use.
			///
			void create(std::string_view slider, std::string_view marker);

			///
			/// Triggered upon mouse movement.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme) noexcept;

			///
			/// Triggered upon mouse click.
			///
			/// \param mpe Mouse Pressed Event.
			///
			void on_event(const events::MousePressed& mpe) noexcept;

			///
			/// Triggered upon mouse release.
			///
			/// \param mre Mouse Released Event.
			///
			void on_event(const events::MouseReleased& mre) noexcept;

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			/// \param dt Delta Time from gameloop.
			///
			void update(const double dt) override;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			void render() override;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Get current value of slider.
			///
			/// \return const float of value. 0.0f - 1.0f. I.e. 0.78f is 78%.
			///
			[[nodiscard]] const float value() const noexcept;

			///
			/// Returns current progress of bar in a percentage.
			///
			/// \return const float progress as pertentage.
			///
			[[nodiscard]] const float percentage() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Slider(const Slider&) = delete;

			///
			/// Move constructor.
			///
			Slider(Slider&&) = delete;

			///
			/// Copy assignment operator.
			///
			Slider& operator=(const Slider&) = delete;

			///
			/// Move assignment operator.
			///
			Slider& operator=(Slider&&) = delete;

		private:
			///
			/// Current value of this bar. 0.0f - 1.0f. I.e. 0.78f is 78%.
			///
			float m_value;

			///
			/// Mouse button is pressed.
			///
			bool m_pressed;

			///
			/// Bounds of marker on bar.
			///
			math::Rect<float> m_marker_pos;

			///
			/// Slider texture.
			///
			components::BatchSprite m_slider;

			///
			/// Slider transform.
			///
			components::Transform2D m_slider_transform;

			///
			/// Marker texture.
			///
			components::BatchSprite m_marker;

			///
			/// Marker transform.
			///
			components::Transform2D m_marker_transform;
		};
	} // namespace ui
} // namespace galaxy

#endif