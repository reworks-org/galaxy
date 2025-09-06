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
	///
	/// Window content scale change.
	///
	struct ContentScale final
	{
		///
		/// The new x - axis content scale of the window.
		///
		float m_xscale = 0.0f;

		///
		/// The new y - axis content scale of the window.
		///
		float m_yscale = 0.0f;
	};
} // namespace galaxy

#endif
