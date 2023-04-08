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
			std::string title;

			///
			/// Width of window.
			///
			int width;

			///
			/// Height of window.
			///
			int height;

			///
			/// Vertical sync.
			///
			bool vsync;

			///
			/// Open window maximized.
			///
			bool maximized;

			///
			/// Enable debug api calls.
			///
			bool debug;

			///
			/// Window content area should be resized based on the monitor content scale.
			///
			bool scale_to_monitor;
		};
	} // namespace core
} // namespace galaxy

#endif