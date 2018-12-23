///
/// Button.hpp
/// starlight
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_BUTTON_HPP_
#define STARLIGHT_BUTTON_HPP_

#include <array>

#include "sl/ui/Widget.hpp"
#include "sl/events/MousePressedEvent.hpp"
#include "sl/events/MouseReleasedEvent.hpp"

namespace sl
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
		/// \param images Array of texture names to load. Need to be all the same dimensions.
		///			textures[0] is default state, textures[1] is pressed state and textures[2] is mouse over (hover) state.				   
		///
		Button(const int x, const int y, const std::array<std::string, 3>& textures);

		///
		/// Text Constructor.
		///
		/// \param x x-pos, relative to the panel.
		/// \param y y-pos, relative to the panel.
		/// \param text Text of the button.
		/// \param font Font of the text.
		/// \param colours Array of colours for each state. 
		///					colors[0] is default state, colors[1] is pressed state and colors[2] is mouse over (hover) state.	
		///
		Button(const int x, const int y, const std::string& text, const std::string& font, const std::array<ALLEGRO_COLOR, 3>& colors);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		///
		Button(const sol::table& table);

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
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void receivePress(const sl::MousePressedEvent& e);

		///
		/// \brief Allows for button to recieve MouseReleasedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseReleasedEvent object.
		///
		void receiveRelease(const sl::MouseReleasedEvent& e);

		///
		/// \brief Allows for button to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void recieveMoved(const sl::MouseMovedEvent& e);

		///
		/// Register callback function when button is pressed.
		///
		/// \param callback Return type void, no arguments. Function pointer.
		///
		void registerCallback(const std::function<void()>& callback);

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
	};
}

#endif