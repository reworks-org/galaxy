///
/// ContentScale.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_CONTENTSCALE_HPP_
#define GALAXY_EVENTS_CONTENTSCALE_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// \brief Window content scale change.
		///
		/// Does not have an "handled" member, since all listeners are expected to handle this.
		///
		struct ContentScale final
		{
			///
			/// The new x - axis content scale of the window.
			///
			float xscale = 0.0f;

			///
			/// The new y - axis content scale of the window.
			///
			float yscale = 0.0f;
		};
	} // namespace events
} // namespace galaxy

#endif