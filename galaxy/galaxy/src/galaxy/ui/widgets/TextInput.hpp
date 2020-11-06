///
/// TextInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TEXTINPUT_HPP_
#define GALAXY_TEXTINPUT_HPP_

#include <protostar/events/KeyDownEvent.hpp>
#include <protostar/events/MouseMovedEvent.hpp>
#include <protostar/events/MousePressedEvent.hpp>
#include <protostar/system/Time.hpp>
#include <qs/shapes/Line.hpp>
#include <qs/sprite/BatchedSprite.hpp>
#include <qs/text/Text.hpp>

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
		class TextInput final : public galaxy::Widget, public qs::BatchedSprite
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			TextInput();

			///
			/// Copy constructor.
			///
			TextInput(const TextInput&) = delete;

			///
			/// Move constructor.
			///
			TextInput(TextInput&&) = default;

			///
			/// Copy assignment operator.
			///
			TextInput& operator=(const TextInput&) = delete;

			///
			/// Move assignment operator.
			///
			TextInput& operator=(TextInput&&) = default;

			///
			/// Destructor.
			///
			virtual ~TextInput();

			///
			/// \brief Create textinput.
			///
			/// Needs "text" shader and "line" shader.
			///
			/// \param textinput Texture of the input field.
			/// \param font Font for the inputted text.
			/// \param border_width Border on texture around input field.
			///
			void create_from_atlas(std::string_view textinput, std::string_view font, float border_width);

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
			/// Triggered upon key down.
			///
			/// \param kde Key Down Event.
			///
			void on_event(const pr::KeyDownEvent& kde);

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
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept override;

			///
			/// Stops all input. Equivalent to pressing the ENTER key.
			///
			void stop();

		private:
			///
			/// Bool to draw text input cursor or not.
			///
			bool m_draw_cursor;

			///
			/// Border width of input field.
			///
			float m_border_width;

			///
			/// Indicator line in input field.
			///
			qs::Line m_cursor;

			///
			/// Text drawn as typed.
			///
			qs::Text m_text;

			///
			/// String containing text input.
			///
			std::string* m_text_input;

			///
			/// Is widget focus.
			///
			bool m_is_focus;

			///
			/// Timer for cursor blinking.
			///
			pr::Timer m_timer;
		};
	} // namespace widget
} // namespace galaxy

#endif