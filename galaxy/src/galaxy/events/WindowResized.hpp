///
/// WindowResized.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_WINDOWRESIZED_HPP_
#define GALAXY_EVENTS_WINDOWRESIZED_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct WindowResized final
		{
			///
			/// Default constructor.
			///
			WindowResized() noexcept;

			///
			/// Constructor.
			///
			/// \param new_width New width of resized window.
			/// \param new_height New height of resized window.
			///
			WindowResized(const int new_width, const int new_height) noexcept;

			///
			/// Default destructor.
			///
			~WindowResized() noexcept = default;

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