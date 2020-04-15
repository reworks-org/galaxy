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
	/// SDL2 compatible window flags.
	///
	unsigned int WindowSettings::s_windowFlags = 0;

	///
	/// Should Multisample Anti-Aliasing be enabled?
	///
	bool WindowSettings::s_msaa = false;

	///
	/// Level of MSAA. I.e. 2, 4, etc.
	///
	int WindowSettings::s_msaaLevel = 2;

	///
	/// Hardware (GPU) or software (CPU) based rendering.
	///
	bool WindowSettings::s_hardwareRendering = true;

	///
	/// Vertical sync.
	///
	bool WindowSettings::s_vsync = false;
}