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
			/// Vertical sync.
			///
			bool m_vsync;

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
			/// Raw mouse input.
			///
			bool m_raw_mouse_input;

			///
			/// OpenGL debug mode.
			///
			bool m_gl_debug;

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
		};
	} // namespace core
} // namespace galaxy

#endif