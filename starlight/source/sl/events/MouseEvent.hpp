///
/// MouseEvent.hpp
/// starlight
///
/// Created by reworks on 20/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_MOUSEEVENT_HPP_
#define STARLIGHT_MOUSEEVENT_HPP_

namespace sl
{
	///
	/// Represents an event that is sent when the mouse triggers an event.
	///
	struct MouseEvent
	{
		///
		/// Current x position of mouse.
		///
		unsigned int x;

		///
		/// Current y position of mouse.
		///
		unsigned int y;
	};
}

#endif