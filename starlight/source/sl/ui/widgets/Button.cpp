///
/// Button.cpp
/// starlight
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Button.hpp"

namespace sl
{
	Button::Button(const sl::Rect<int>& bounds)
		:Widget(bounds)
	{
		Locator::dispatcher->sink<>().connect(this);
	}

	void Button::render()
	{
	}

	void Button::receive(const MouseEvent& e)
	{
	}
}