///
/// Slider.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SLIDER_HPP_
#define GALAXY_SLIDER_HPP_

#include <protostar/events/MouseMovedEvent.hpp>
#include <protostar/events/MousePressedEvent.hpp>
#include <protostar/events/MouseReleasedEvent.hpp>
#include <qs/sprite/BatchedSprite.hpp>

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		///
		/// UI bar that can be moved to increment and decrement a value.
		///
		class Slider final : public galaxy::Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Slider();

			///
			/// Copy constructor.
			///
			Slider(const Slider&) = delete;

			///
			/// Move constructor.
			///
			Slider(Slider&&) = default;

			///
			/// Copy assignment operator.
			///
			Slider& operator=(const Slider&) = delete;

			///
			/// Move assignment operator.
			///
			Slider& operator=(Slider&&) = default;

			///
			/// Destructor.
			///
			virtual ~Slider() = default;

			///
			/// Create slider.
			///
			/// \param slider Texture of the slider in theme to use.
			/// \param marker Texture of the marker in theme to use.
			/// \param theme Theme of the widget to use.
			///
			void create(std::string_view slider, std::string_view marker);

			///
			/// Triggered upon mouse movement.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const pr::MouseMovedEvent& mme);

			///
			/// Triggered upon mouse click.
			///
			/// \param mpe Mouse Pressed Event.
			///
			void on_event(const pr::MousePressedEvent& mpe);

			///
			/// Triggered upon mouse release.
			///
			/// \param mre Mouse Released Event.
			///
			void on_event(const pr::MouseReleasedEvent& mre);

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			void update() override;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			/// \param camera View camera to use when rendering.
			///
			void render(qs::Camera& camera) override;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y);

			///
			/// Get current value of slider.
			///
			/// \return const int of value. 0.0f - 1.0f. I.e. 0.78f is 78%.
			///
			const float value() const;

			///
			/// Returns current progress of bar in a percentage.
			///
			/// \return const int progress as pertentage.
			///
			const float percentage() const;

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
			pr::Rect<float> m_marker_pos;

			///
			/// Slider texture.
			///
			qs::BatchedSprite m_slider;

			///
			/// Marker texture.
			///
			qs::BatchedSprite m_marker;
		};
	} // namespace widget
} // namespace galaxy

#endif