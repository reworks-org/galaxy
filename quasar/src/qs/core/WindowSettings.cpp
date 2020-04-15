///
/// WindowSettings.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "WindowSettings.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Level of MSAA. 0, 2, 4, etc...
	///
	int WindowSettings::s_msaa = 0;

	///
	/// Vertical sync.
	///
	bool WindowSettings::s_vsync = false;

	///
	/// sRGB support.
	///
	bool WindowSettings::s_srgb = false;

	///
	/// Aspect Ratio x.
	///
	int WindowSettings::s_aspectRatioX = 16;

	///
	/// Aspect Ratio y.
	///
	int WindowSettings::s_aspectRatioY = 9;

	///
	/// Raw mouse input.
	///
	bool WindowSettings::s_rawMouseInput = true;
}