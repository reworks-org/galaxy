///
/// Button.hpp
/// galaxy
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_BUTTON_HPP_
#define GALAXY_BUTTON_HPP_

#include <array>

#include "galaxy/ui/Widget.hpp"
#include "galaxy/math/Vector2.hpp"
#include "galaxy/events/MousePressedEvent.hpp"
#include "galaxy/events/MouseReleasedEvent.hpp"

galaxy
{
	///
	/// Creates a clickable button.
	///
	class Button final : public Widget
	{
		///
		/// Defines a state for the button.
		///
		enum class State
		{
			///
			/// When the mouse is not pressing or hovering over the button.
			///
			DEFAULT = 0,

			///
			/// When it is being pressed by the mouse.
			///
			PRESSED = 1,

			///
			/// When the mouse is hovering over the button.
			///
			HOVER = 2
		};

	public:
		///
		/// Texture Constructor.
		///
		/// \param x x-pos of button relative to panel.
		/// \param y y-pos of button relative to panel.
		/// \param label Label to place on the button.
		/// \param textures Array of texture names in theme to use. Need to be all the same dimensions.
		///			textures[0] is default state, textures[1] is pressed state and textures[2] is mouse over (hover) state.		
		/// \param theme Theme for this widget.
		///
		Button(const int x, const int y, const std::string& label, const std::array<std::string, 3>& textures, UITheme* theme);

		///
		/// Text Constructor.
		///
		/// \param x x-pos, relative to the panel.
		/// \param y y-pos, relative to the panel.
		/// \param text Text of the button.
		/// \param colours Array of colours for each state. 
		///					colors[0] is default state, colors[1] is pressed state and colors[2] is mouse over (hover) state.	
		/// \param theme Theme for this widget.
		///
		Button(const int x, const int y, const std::string& text, const std::array<ALLEGRO_COLOR, 3>& colours, UITheme* theme);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme for this widget.
		///
		Button(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~Button() noexcept;

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
		/// \brief Allows for button to recieve MouseReleasedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (galaxyLocator::dispatcher).
		///
		/// \param e MouseReleasedEvent object.
		///
		void receiveRelease(const galaxyMouseReleasedEvent& e);

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
		Button() = delete;

	private:
		///
		/// Current state of the button.
		///
		Button::State m_state;

		///
		/// textures for each button state.
		///
		std::array<ALLEGRO_BITMAP*, 3> m_textures;

		///
		/// Callback function.
		///
		std::function<void()> m_callback;

		///
		/// Label for button.
		///
		std::string m_label;

		///
		/// Button label x, y coords.
		///
		galaxyVector2<int> m_labelPos;

	};
}

#endif