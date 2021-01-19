///
/// WindowSettings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOWSETTINGS_HPP_
#define GALAXY_CORE_WINDOWSETTINGS_HPP_

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
			/// Level of MSAA. I.e. 2, 4, etc.
			///
			int m_anti_aliasing;

			///
			/// Level of ansiotropic filtering.
			///
			int m_ansio_filtering;

			///
			/// Vertical sync.
			///
			bool m_vsync;

			///
			/// sRGB support.
			///
			bool m_srgb;

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
			/// Line thickness.
			///
			float m_line_thickness;

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
		};
	} // namespace core
} // namespace galaxy

#endif