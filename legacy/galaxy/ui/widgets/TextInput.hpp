///
/// TextInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_TEXTINPUT_HPP_
#define GALAXY_UI_WIDGETS_TEXTINPUT_HPP_

#include "galaxy/async/Timer.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/graphics/shapes/Line.hpp"
#include "galaxy/graphics/sprite/BatchedSprite.hpp"
#include "galaxy/graphics/text/Text.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// UI bar that can be moved to increment and decrement a value.
		///
		class TextInput final : public Widget, public graphics::BatchedSprite
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
			/// Copy assignment operator.
			///
			TextInput& operator=(const TextInput&) = delete;

			///
			/// Destructor.
			///
			virtual ~TextInput();

			///
			/// \brief Create textinput.
			///
			/// Needs "glyph" shader and "line" shader.
			///
			/// \param textinput Texture of the input field.
			/// \param font Font for the inputted text.
			/// \param border_width Border on texture around input field.
			///
			using graphics::BatchedSprite::create;
			void create(std::string_view textinput, std::string_view font, float border_width);

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
			/// Triggered upon key down.
			///
			/// \param kde Key Down Event.
			///
			void on_event(const events::KeyDown& kde);

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

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) override;

			///
			/// Stops all input. Equivalent to pressing the ENTER key.
			///
			void stop();

		private:
			///
			/// Total characters allowed in input field.
			///
			int m_total_chars;

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
			graphics::Line m_cursor;

			///
			/// Text drawn as typed.
			///
			graphics::Text m_text;

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
			async::Timer m_timer;
		};
	} // namespace ui
} // namespace galaxy

#endif