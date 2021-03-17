///
/// Textbox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_TEXTBOX_HPP_
#define GALAXY_UI_WIDGETS_TEXTBOX_HPP_

#include "galaxy/async/Timer.hpp"
#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// JRPG style textbox.
		///
		class Textbox final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Textbox() noexcept;

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
			void create(std::string_view box, std::string_view indicator, std::string_view font, float border_width);

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme) noexcept;

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
			void render() override;

			///
			/// Set textbox messages.
			///
			/// \param messages A list of messages to display on the textbox. Each element should be a new message.
			///
			void set_text(std::span<std::string> messages) noexcept;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Textbox(const Textbox&) = delete;

			///
			/// Move constructor.
			///
			Textbox(Textbox&&) = delete;

			///
			/// Copy assignment operator.
			///
			Textbox& operator=(const Textbox&) = delete;

			///
			/// Move assignment operator.
			///
			Textbox& operator=(Textbox&&) = delete;

		private:
			///
			/// Box sprite.
			///
			components::BatchSprite m_box;

			///
			/// Box Transform.
			///
			components::Transform m_box_transform;

			///
			/// Text to render.
			///
			components::Text m_text;

			///
			/// Text transform.
			///
			components::Transform m_text_transform;

			///
			/// Indicator.
			///
			components::BatchSprite m_indicator;

			///
			/// Indicator transform.
			///
			components::Transform m_indicator_transform;

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
			/// Text shader cache.
			///
			graphics::Shader* m_shader;
		};
	} // namespace ui
} // namespace galaxy

#endif