///
/// Textbox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_TEXTBOX_HPP_
#define GALAXY_UI_WIDGETS_TEXTBOX_HPP_

#include "galaxy/async/Timer.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/graphics/sprite/BatchedSprite.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// JRPG style textbox.
		///
		class Textbox final : public Widget, public graphics::BatchedSprite
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Textbox();

			///
			/// Copy constructor.
			///
			Textbox(const Textbox&) = delete;

			///
			/// Copy assignment operator.
			///
			Textbox& operator=(const Textbox&) = delete;

			///
			/// Destructor.
			///
			virtual ~Textbox();

			///
			/// \brief Create textinput.
			///
			/// Needs "text" shader and "line" shader.
			///
			/// \param box Texture of the textbox.
			/// \param indicator Texture for the continue indicator.
			/// \param font Font for the displayed text.
			/// \param border_width Border around textbox - text will be rendered centered inside.
			///
			using graphics::BatchedSprite::create;
			void create(std::string_view box, std::string_view indicator, std::string_view font, float border_width);

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme);

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
			/// Set textbox messages.
			///
			/// \param messages A list of messages to display on the textbox. Each element should be a new message.
			///
			void set_text(std::span<std::string> messages);

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) override;

		private:
			///
			/// Text to render.
			///
			graphics::Text m_text;

			///
			/// Indicator.
			///
			graphics::BatchedSprite m_indicator;

			///
			/// Text to display in textbox.
			///
			std::string m_text_to_draw;

			///
			/// All the text to draw.
			///
			std::vector<std::string> m_messages;

			///
			/// Border width.
			///
			float m_border_width;

			///
			/// Timer for indicator.
			///
			async::Timer m_indicator_timer;

			///
			/// Timer for text.
			///
			async::Timer m_draw_text_timer;

			///
			/// Current index of textbox messages.
			///
			std::size_t m_messages_index;

			///
			/// Current index in message string.
			///
			std::size_t m_char_index;

			///
			/// Toggle to change position of indicator.
			///
			bool m_draw_lower;

			///
			/// Previous text.
			///
			std::string m_prev_text;

			///
			/// Indicator x.
			///
			float m_ind_x;

			///
			/// Indicator y.
			///
			float m_ind_y;

			///
			/// Prevents race conditions.
			///
			std::mutex m_mutex;
		};
	} // namespace ui
} // namespace galaxy

#endif