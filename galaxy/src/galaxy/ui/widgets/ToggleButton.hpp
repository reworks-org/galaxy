///
/// ToggleButton.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOGGLEBUTTON_HPP_
#define GALAXY_TOGGLEBUTTON_HPP_

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
		/// \brief Simple ToggleButton widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class ToggleButton final : public galaxy::Widget, public qs::BatchedSprite
		{
		public:
			///
			/// Determines current button state.
			///
			enum class State : short
			{
				ON,
				OFF,
				ON_HOVER,
				OFF_HOVER
			};

			///
			/// Constructor.
			///
			ToggleButton();

			///
			/// Copy constructor.
			///
			ToggleButton(const ToggleButton&) = delete;

			///
			/// Move constructor.
			///
			ToggleButton(ToggleButton&&) = default;

			///
			/// Copy assignment operator.
			///
			ToggleButton& operator=(const ToggleButton&) = delete;

			///
			/// Move assignment operator.
			///
			ToggleButton& operator=(ToggleButton&&) = default;

			///
			/// Destructor.
			///
			virtual ~ToggleButton() = default;

			///
			/// Create the button widget.
			///
			/// \param on Texture when button is on.
			/// \param off Texture when button is off.
			/// \param on_hover Texture when button is on and mouse is hovering.
			/// \param off_hover Texture when button is off and mouse is hovering.
			///
			using qs::BatchedSprite::create;
			void create(std::string_view on, std::string_view off, std::string_view on_hover, std::string_view off_hover);

			///
			/// Set the callback function.
			///
			/// \param func Function must be void return type and take a single bool parameter.
			///
			template<typename Callback>
			void set_callback(Callback&& func);

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) override;

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
			/// \param camera View camera to use when rendering.
			///
			void render(qs::Camera& camera) override;

			///
			/// Check if togglebutton is toggled on.
			///
			/// \return Const boolean - true if toggled on.
			///
			[[nodiscard]] const bool is_on() const;

		private:
			///
			/// Check if togglebutton is on.
			///
			bool m_on;

			///
			/// ToggleButton state.
			///
			ToggleButton::State m_state;

			///
			/// Callback.
			///
			std::function<void(bool)> m_callback;

			///
			/// Contains each region on the texture atlas.
			///
			std::array<pr::Rect<float>, 4> m_regions;
		};

		template<typename Callback>
		inline void ToggleButton::set_callback(Callback&& func)
		{
			m_callback = std::move(func);
		}
	} // namespace widget
} // namespace galaxy

#endif