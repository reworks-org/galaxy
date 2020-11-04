///
/// Button.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_BUTTON_HPP_
#define GALAXY_BUTTON_HPP_

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
		/// \brief Simple Button widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Button final : public galaxy::Widget, public qs::BatchedSprite
		{
		public:
			///
			/// Determines current button state.
			///
			enum class State : short
			{
				DEFAULT,
				PRESSED,
				HOVER
			};

			///
			/// Constructor.
			///
			Button();

			///
			/// Copy constructor.
			///
			Button(const Button&) = delete;

			///
			/// Move constructor.
			///
			Button(Button&&) = default;

			///
			/// Copy assignment operator.
			///
			Button& operator=(const Button&) = delete;

			///
			/// Move assignment operator.
			///
			Button& operator=(Button&&) = default;

			///
			/// Destructor.
			///
			virtual ~Button() = default;

			///
			/// Create the button widget.
			///
			/// \param sprites [0] is Default, [1] is Pressed, [2] is hover.
			///
			void create_from_atlas(const std::array<std::string, 3>& sprites);

			///
			/// Set the callback function.
			///
			/// \param func Function must be void return type and no params.
			///
			template<typename Callback>
			void set_callback(Callback&& func);

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept override;

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

		private:
			///
			/// Button state.
			///
			Button::State m_state;

			///
			/// Callback.
			///
			std::function<void(void)> m_callback;

			///
			/// Contains each region on the texture atlas.
			///
			std::array<pr::Rect<float>, 3> m_regions;
		};

		template<typename Callback>
		inline void Button::set_callback(Callback&& func)
		{
			m_callback = std::move(func);
		}
	} // namespace widget
} // namespace galaxy

#endif