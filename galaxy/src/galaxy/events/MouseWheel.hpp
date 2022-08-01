///
/// MouseWheel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEWHEEL_HPP_
#define GALAXY_EVENTS_MOUSEWHEEL_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains mouse wheel movement data.
		///
		struct MouseWheel final
		{
			///
			/// -1 or 1.
			///
			double m_xoff;

			///
			/// -1 or 1.
			///
			double m_yoff;
		};
	} // namespace events
} // namespace galaxy

#endif