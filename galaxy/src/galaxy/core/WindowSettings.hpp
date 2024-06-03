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
			/// Width of window (or fullscreen).
			///
			int window_width;

			///
			/// Height of window (or fullscreen).
			///
			int window_height;

			///
			/// The virtual or final framebuffer width.
			///
			int frame_width;

			///
			/// The virtual or final framebuffer height.
			///
			int frame_height;

			///
			/// Vertical sync.
			///
			bool vsync;

			///
			/// Open window maximized.
			///
			bool maximized;

			///
			/// Open window fullscreen. Has priority over maximized.
			///
			bool fullscreen;

			///
			/// Enable debug api calls.
			///
			bool debug;
		};
	} // namespace core
} // namespace galaxy

#endif
