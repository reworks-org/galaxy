///
/// ToggleButton.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_TOGGLEBUTTON_HPP_
#define GALAXY_UI_WIDGETS_TOGGLEBUTTON_HPP_

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
		/// \brief Simple ToggleButton widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class ToggleButton final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Determines current button state.
			///
			enum class State : short
			{
				ON,
				OFF,
				ON_HOVER,
				OFF_HOVER
			};

			///
			/// Constructor.
			///
			ToggleButton() noexcept;

			///
			/// Destructor.
			///
			virtual ~ToggleButton() noexcept = default;

			///
			/// Create the button widget.
			///
			/// \param on Texture when button is on.
			/// \param off Texture when button is off.
			/// \param on_hover Texture when button is on and mouse is hovering.
			/// \param off_hover Texture when button is off and mouse is hovering.
			///
			void create(std::string_view on, std::string_view off, std::string_view on_hover, std::string_view off_hover);

			///
			/// Set the callback function.
			///
			/// \param func Function must be void return type and take a single bool parameter.
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
			/// Check if togglebutton is toggled on.
			///
			/// \return Const boolean - true if toggled on.
			///
			[[nodiscard]] const bool is_on() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			ToggleButton(const ToggleButton&) = delete;

			///
			/// Move constructor.
			///
			ToggleButton(ToggleButton&&) = delete;

			///
			/// Copy assignment operator.
			///
			ToggleButton& operator=(const ToggleButton&) = delete;

			///
			/// Move assignment operator.
			///
			ToggleButton& operator=(ToggleButton&&) = delete;

		private:
			///
			/// Check if togglebutton is on.
			///
			bool m_on;

			///
			/// ToggleButton state.
			///
			ToggleButton::State m_state;

			///
			/// Callback.
			///
			std::function<void(bool)> m_callback;

			///
			/// Contains each region on the texture atlas.
			///
			std::array<graphics::fRect, 4> m_regions;

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
		inline void ToggleButton::set_callback(Callback&& func)
		{
			m_callback = std::move(func);
		}
	} // namespace ui
} // namespace galaxy

#endif