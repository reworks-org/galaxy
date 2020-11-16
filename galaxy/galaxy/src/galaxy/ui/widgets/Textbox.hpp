///
/// Textbox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TEXTBOX_HPP_
#define GALAXY_TEXTBOX_HPP_

#include <protostar/events/MouseMovedEvent.hpp>
#include <protostar/events/KeyDownEvent.hpp>
#include <protostar/system/Time.hpp>
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
		/// JRPG style textbox.
		///
		class Textbox final : public galaxy::Widget, public qs::BatchedSprite
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
			/// Move constructor.
			///
			Textbox(Textbox&&) = default;

			///
			/// Copy assignment operator.
			///
			Textbox& operator=(const Textbox&) = delete;

			///
			/// Move assignment operator.
			///
			Textbox& operator=(Textbox&&) = default;

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
			using qs::BatchedSprite::create;
			void create(std::string_view box, std::string_view indicator, std::string_view font, float border_width);

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const pr::MouseMovedEvent& mme);

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
			void set_pos(const float x, const float y) noexcept override;

		private:
			///
			/// Text to render.
			///
			qs::Text m_text;

			///
			/// Indicator.
			///
			qs::BatchedSprite m_indicator;

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
			pr::Timer m_indicator_timer;

			///
			/// Timer for text.
			///
			pr::Timer m_draw_text_timer;

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
	} // namespace widget
} // namespace galaxy

#endif