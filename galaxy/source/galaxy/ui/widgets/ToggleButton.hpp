///
/// ToggleButton.hpp
/// galaxy
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOGGLEBUTTON_HPP_
#define GALAXY_TOGGLEBUTTON_HPP_

#include <array>

#include "galaxy/ui/Widget.hpp"
#include "galaxy/events/MouseMovedEvent.hpp"
#include "galaxy/events/MousePressedEvent.hpp"
#include "galaxy/events/MouseReleasedEvent.hpp"

galaxy
{
	///
	/// Creates a button that toggles from one state to another.
	//  Can be used to create check boxes or radial ui objects.
	///
	class ToggleButton final : public Widget
	{
		///
		/// Defines a state for the button.
		///
		enum class State
		{
			///
			/// When the button is in an off state.
			///
			OFF = 0,

			///
			/// When the button is in an on state.
			///
			ON = 1,

			///
			/// When the button is being hovered on.
			///
			HOVER = 2
		};

	public:
		///
		/// Texture Constructor.
		///
		/// \param x x-pos relative to panel.
		/// \param y y-pos relative to panel.
		/// \param textures Array of texture names in theme to use. Must be the same width and height.
		///			textures[0] is OFF state, textures[1] is ON state and textures[2] is mouse over (hover) state.
		/// \param theme Theme for this widget.
		///
		ToggleButton(const int x, const int y, const std::array<std::string, 3>& textures, UITheme* theme);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme for this widget.
		///
		ToggleButton(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~ToggleButton() noexcept;

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		void render() override;

		///
		/// \brief Allows for button to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receivePress(const galaxyMousePressedEvent& e);

		///
		/// \brief Allows for button to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void recieveMoved(const galaxyMouseMovedEvent& e);

		///
		/// Register callback function when button is pressed.
		///
		/// \param callback Return type void, no arguments. Function pointer.
		///
		void registerCallback(const std::function<void()>& callback);

		///
		/// \brief Set the offset of the widget from the panel. Called for you in the Panel::add widget function.
		///
		/// It should look like this:
		/// m_bounds.m_x += x;
		/// m_bounds.m_y += y;
		///
		/// \param x x-pos of the panel.
		/// \param y y-pos of the panel.
		///
		void setOffset(const int x, const int y) override;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		ToggleButton() = delete;

	private:
		///
		/// Current state of the button.
		///
		ToggleButton::State m_state;

		///
		/// Images for each button state.
		///
		std::array<ALLEGRO_BITMAP*, 3> m_textures;

		///
		/// Callback function.
		///
		std::function<void()> m_callback;
	};
}

#endif