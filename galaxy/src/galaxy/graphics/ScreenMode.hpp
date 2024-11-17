///
/// ScreenMode.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SCREENMODE_HPP_
#define GALAXY_GRAPHICS_SCREENMODE_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Window fullscreen mode type.
		///
		enum class ScreenMode : int
		{
			NONE       = 0,
			MAXIMIZED  = 1,
			FULLSCREEN = 2,
			BORDERLESS = 3
		};
	} // namespace graphics
} // namespace galaxy

#endif
