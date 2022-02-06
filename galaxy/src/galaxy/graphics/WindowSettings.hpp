///
/// WindowSettings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_WINDOWSETTINGS_HPP_
#define GALAXY_GRAPHICS_WINDOWSETTINGS_HPP_

#include <string>

namespace galaxy
{
	namespace graphics
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
			/// Open window maximized.
			///
			bool m_maximized;

			///
			/// Aspect Ratio x.
			/// Set to -1 to ignore.
			///
			int m_aspect_ratio_x;

			///
			/// Aspect Ratio y.
			/// Set to -1 to ignore.
			///
			int m_aspect_ratio_y;

			///
			/// Vertical sync.
			///
			bool m_vsync;

			///
			/// Enable debug api calls.
			///
			bool m_debug;
		};
	} // namespace graphics
} // namespace galaxy

#endif