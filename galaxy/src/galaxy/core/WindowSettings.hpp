///
/// WindowSettings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOWSETTINGS_HPP_
#define GALAXY_CORE_WINDOWSETTINGS_HPP_

#include <string>

namespace galaxy
{
	namespace core
	{
		///
		/// Holds window related settings.
		///
		struct WindowSettings final
		{
			///
			/// Window title.
			///
			std::string m_title;

			///
			/// Width of window.
			///
			int m_width;

			///
			/// Height of window.
			///
			int m_height;

			///
			/// Vertical sync.
			///
			bool m_vsync;

			///
			/// Open window maximized.
			///
			bool m_maximized;

			///
			/// Enable debug api calls.
			///
			bool m_debug;

			///
			/// Window content area should be resized based on the monitor content scale.
			///
			bool m_scale_to_monitor;
		};
	} // namespace core
} // namespace galaxy

#endif