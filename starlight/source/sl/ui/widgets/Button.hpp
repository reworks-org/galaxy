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

#include "sl/ui/Widget.hpp"
#include "sl/events/MouseEvent.hpp"

namespace sl
{
	class Button final : public Widget
	{
		friend class entt::Dispatcher;

		///
		/// Defines a state for the button.
		///
		enum class State
		{
			BUTTON_PRESSED,
			BUTTON_RELEASED,
			BUTTON_HOVER
		};

	public:
		///
		/// Constructor.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		explicit Button(const sl::Rect<int>& bounds);

		///
		/// \brief Render the Widget.
		///
		/// This should only contain code on rendering the widget. Remember to check for visibility.
		///
		void render() override;

	private:
		///
		/// \brief Allows for buttons to recieve mouse events. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		void receive(const MouseEvent& e);
	};
}

#endif