///
/// Label.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_LABEL_HPP_
#define GALAXY_UI_WIDGETS_LABEL_HPP_

#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// \brief Simple Label widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Label final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Label() noexcept;

			///
			/// Destructor.
			///
			virtual ~Label() noexcept;

			///
			/// \brief Create the label widget.
			///
			/// Need a 'glyph' shader in shaderbook.
			///
			/// \param text Label to display.
			/// \param font Font to use.
			///
			void create(std::string_view text, std::string_view font);

			///
			/// Update the text.
			///
			/// \param text New text to display on label.
			///
			void update_text(std::string_view text);

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme) noexcept;

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
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Label(const Label&) = delete;

			///
			/// Move constructor.
			///
			Label(Label&&) = delete;

			///
			/// Copy assignment operator.
			///
			Label& operator=(const Label&) = delete;

			///
			/// Move assignment operator.
			///
			Label& operator=(Label&&) = delete;

		private:
			///
			/// Label to render.
			///
			components::Text m_text;

			///
			/// Label transform.
			///
			components::Transform m_transform;

			///
			/// Text shader.
			///
			graphics::Shader* m_shader;
		};
	} // namespace ui
} // namespace galaxy

#endif