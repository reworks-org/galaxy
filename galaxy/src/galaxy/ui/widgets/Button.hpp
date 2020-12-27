///
/// Button.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_BUTTON_HPP_
#define GALAXY_UI_WIDGETS_BUTTON_HPP_

#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/graphics/sprite/BatchedSprite.hpp"
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
		class Button final : public Widget, public graphics::BatchedSprite
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
			/// Copy assignment operator.
			///
			Button& operator=(const Button&) = delete;

			///
			/// Destructor.
			///
			virtual ~Button() = default;

			///
			/// Create the button widget.
			///
			/// \param def Texture when button is not interacting with mouse.
			/// \param pressed Texture when button is being pressed.
			/// \param hover Texture when mouse is hovering over widget.
			///
			using graphics::BatchedSprite::create;
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
			void set_pos(const float x, const float y) override;

			///
			/// Triggered upon mouse movement.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme);

			///
			/// Triggered upon mouse click.
			///
			/// \param mpe Mouse Pressed Event.
			///
			void on_event(const events::MousePressed& mpe);

			///
			/// Triggered upon mouse release.
			///
			/// \param mre Mouse Released Event.
			///
			void on_event(const events::MouseReleased& mre);

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
			void render(graphics::Camera& camera) override;

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
			std::array<graphics::Rect<float>, 3> m_regions;
		};

		template<typename Callback>
		inline void Button::set_callback(Callback&& func)
		{
			m_callback = std::move(func);
		}
	} // namespace ui
} // namespace galaxy

#endif