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
#include "sl/ui/events/ButtonEvent.hpp"

namespace sl
{
	class Button : public Widget<ButtonEvent>
	{
	public:
		///
		/// Constructor.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		Button(const sl::Rect<int>& bounds);

		///
		/// \brief Subscribe to an event.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		void receive(const ButtonEvent& e)  override;

		///
		/// \brief Render the Widget.
		///
		/// This should only contain code on rendering the widget. Remember to check for visibility.
		///
		void render() override;
	};
}

#endif