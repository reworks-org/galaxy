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
		public:
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
			/// \param x x-pos of widget.
			/// \param y y-pos of widget.
			/// \param slider Texture of the slider in theme to use.
			/// \param marker Texture of the marker in theme to use.
			/// \param theme Theme of the widget to use.
			///
			void create(const int x, const int y, std::string_view slider, std::string_view marker);

			///
			/// Allows for Slider to recieve MouseMovedEvents. Automatically registered with entt.
			///
			/// \param e MouseMovedEvent object.
			///
			void on_event(const pr::MouseMovedEvent& mee);

			///
			/// Allows for Slider to recieve MousePressedEvents. Automatically registered with entt.
			///
			/// \param e MousePressedEvent object.
			///
			void on_event(const pr::MousePressedEvent& mpe);

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