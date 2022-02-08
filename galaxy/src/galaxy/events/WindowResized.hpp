///
/// WindowResized.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_WINDOWRESIZED_HPP_
#define GALAXY_EVENTS_WINDOWRESIZED_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// New width and height of window being resized.
		///
		struct WindowResized final
		{
			///
			/// New width of window.
			///
			int m_width;

			///
			/// New height of window.
			///
			int m_height;
		};
	} // namespace events
} // namespace galaxy

#endif