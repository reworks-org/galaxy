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
		/// \brief New width and height of window being resized.
		///
		/// Does not have an "handled" member, since all listeners are expected to handle this.
		///
		struct WindowResized final
		{
			///
			/// New width of window.
			///
			int m_width = 0;

			///
			/// New height of window.
			///
			int m_height = 0;
		};
	} // namespace events
} // namespace galaxy

#endif