///
/// WindowSettings.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "WindowSettings.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Level of MSAA. 0, 2, 4, etc...
	///
	int WindowSettings::s_anti_aliasing = 2;

	///
	/// Level of ansiotropic filtering.
	///
	int WindowSettings::s_ansio_filtering = 2;

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
	int WindowSettings::s_aspect_ratio_x = 16;

	///
	/// Aspect Ratio y.
	///
	int WindowSettings::s_aspect_ratio_y = 9;

	///
	/// Raw mouse input.
	///
	bool WindowSettings::s_raw_mouse_input = true;

	///
	/// Texture format.
	///
	unsigned int WindowSettings::s_texture_format = GL_RGBA8;
} // namespace qs