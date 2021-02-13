///
/// Button.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_BUTTON_HPP_
#define GALAXY_UI_WIDGETS_BUTTON_HPP_

#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// \brief Simple Button widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Button final : public Widget
		{
			friend class GUI;

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
			Button() noexcept;

			///
			/// Destructor.
			///
			virtual ~Button() noexcept = default;

			///
			/// Create the button widget.
			///
			/// \param def Texture when button is not interacting with mouse.
			/// \param pressed Texture when button is being pressed.
			/// \param hover Texture when mouse is hovering over widget.
			///
			void create(std::string_view def, std::string_view pressed, std::string_view hover);

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
			void set_pos(const float x, const float y) noexcept;

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

		private:
			///
			/// Copy constructor.
			///
			Button(const Button&) = delete;

			///
			/// Move constructor.
			///
			Button(Button&&) = delete;

			///
			/// Copy assignment operator.
			///
			Button& operator=(const Button&) = delete;

			///
			/// Move assignment operator.
			///
			Button& operator=(Button&&) = delete;

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
			std::array<std::string, 3> m_regions;

			///
			/// Batched sprite.
			///
			components::BatchedSprite m_batched;

			///
			/// Batched Transform.
			///
			components::Transform m_transform;
		};

		template<typename Callback>
		inline void Button::set_callback(Callback&& func)
		{
			m_callback = std::move(func);
		}
	} // namespace ui
} // namespace galaxy

#endif