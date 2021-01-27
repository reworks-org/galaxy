///
/// TextInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_TEXTINPUT_HPP_
#define GALAXY_UI_WIDGETS_TEXTINPUT_HPP_

#include "galaxy/async/Timer.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// UI bar that can be moved to increment and decrement a value.
		///
		class TextInput final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			TextInput() noexcept;

			///
			/// Destructor.
			///
			virtual ~TextInput() noexcept;

			///
			/// \brief Create textinput.
			///
			/// Needs "glyph" shader and "line" shader.
			///
			/// \param textinput Texture of the input field.
			/// \param font Font for the inputted text.
			/// \param border_width Border on texture around input field.
			///
			void create(std::string_view textinput, std::string_view font, float border_width);

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
			/// Triggered upon key down.
			///
			/// \param kde Key Down Event.
			///
			void on_event(const events::KeyDown& kde) noexcept;

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
			void set_pos(const float x, const float y) noexcept;

			///
			/// Stops all input. Equivalent to pressing the ENTER key.
			///
			void stop() noexcept;

		private:
			///
			/// Copy constructor.
			///
			TextInput(const TextInput&) = delete;

			///
			/// Move constructor.
			///
			TextInput(TextInput&&) = delete;

			///
			/// Copy assignment operator.
			///
			TextInput& operator=(const TextInput&) = delete;

			///
			/// Move assignment operator.
			///
			TextInput& operator=(TextInput&&) = delete;

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
			/// Batched sprite.
			///
			components::BatchedSprite m_batched;

			///
			/// Batched Transform.
			///
			components::Transform m_transform;

			///
			/// Indicator line in input field.
			///
			components::Line m_cursor;

			///
			/// Cursor transform.
			///
			components::Transform m_cursor_transform;

			///
			/// Text drawn as typed.
			///
			components::Text m_text;

			///
			/// Text transform.
			///
			components::Transform m_text_transform;

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

			///
			/// Cached line shader.
			///
			graphics::Shader* m_line_shader;

			///
			/// Cached text shader.
			///
			graphics::Shader* m_text_shader;
		};
	} // namespace ui
} // namespace galaxy

#endif