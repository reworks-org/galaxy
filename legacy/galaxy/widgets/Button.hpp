///
/// Button.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_BUTTON_HPP_
#define GALAXY_UI_WIDGETS_BUTTON_HPP_

#include "galaxy/components/BatchSprite.hpp"
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
			/// Set the script to call when this button is pressed.
			///
			/// \param script_id ID of the script in the ScriptBook.
			///
			void set_onclick(std::string_view script_id);

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
			/// Script to call on click.
			///
			std::string m_onclick;

			///
			/// Contains each region on the texture atlas.
			///
			std::array<std::string, 3> m_regions;

			///
			/// Batched sprite.
			///
			components::BatchSprite m_sprite;

			///
			/// Batched Transform.
			///
			components::Transform2D m_transform;
		};
	} // namespace ui
} // namespace galaxy

#endif