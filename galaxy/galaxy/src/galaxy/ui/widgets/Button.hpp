///
/// Button.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_BUTTON_HPP_
#define GALAXY_BUTTON_HPP_

#include <protostar/events/MousePressedEvent.hpp>
#include <protostar/events/MouseReleasedEvent.hpp>

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Creates a clickable button.
	///
	class Button final : public galaxy::Widget
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
		/// \param theme Theme for this widget.
		/// \param x x-pos of button.
		/// \param y y-pos of button.
		///
		Button(galaxy::Theme* theme, const int x, const int y) noexcept;

		///
		/// Destructor.
		///
		~Button() noexcept;

		///
		/// \brief Allows for button to recieve MousePressedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MousePressedEvent object.
		///
		void onPress(const protostar::MousePressedEvent& e) noexcept;

		///
		/// \brief Allows for button to recieve MouseReleasedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseReleasedEvent object.
		///
		void onRelease(const protostar::MouseReleasedEvent& e) noexcept;

		///
		/// \brief Allows for button to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void onMove(const protostar::MouseMovedEvent& e) noexcept;

		///
		/// Label for button.
		///
		/// \param label Label to go on text.
		/// \param font Font to draw text with.
		/// \param col Colour of text.
		///
		void createLabel(const std::string& label, std::string_view font, const protostar::Colour& col) noexcept;

		///
		/// Activate widget.
		///
		void activate() noexcept override;

		///
		/// Deactivate widget.
		///
		void deactivate() noexcept override;

		///
		/// \brief Update the widget.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		/// \param dt Delta Time.
		///
		void update(protostar::ProtectedDouble* dt) noexcept override;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param camera Camera projection to apply to GUI.
		///
		void render(qs::Camera& camera) noexcept override;

		///
		/// Check if the button has been pressed.
		///
		/// \return True if button was pressed.
		///
		const bool isPressed() const noexcept;

	private:
		///
		/// Current state of the button.
		///
		Button::State m_state;

		///
		/// Is button pressed.
		///
		bool m_pressed;

		///
		/// Label for button.
		///
		std::unique_ptr<qs::Text> m_text;

		///
		/// Sprite of button.
		///
		qs::BatchedSprite m_sprite;

		///
		/// Bounds.
		///
		protostar::Rect<int> m_bounds;
	};
}

#endif