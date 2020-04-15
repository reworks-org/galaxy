///
/// WindowSettings.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOWSETTINGS_HPP_
#define QUASAR_WINDOWSETTINGS_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Holds window related settings.
	///
	class WindowSettings final
	{
	public:
		///
		/// SDL2 compatible window flags.
		///
		static unsigned int s_windowFlags;

		///
		/// Should Multisample Anti-Aliasing be enabled?
		///
		static bool s_msaa;

		///
		/// Level of MSAA. I.e. 2, 4, etc.
		///
		static int s_msaaLevel;

		///
		/// Hardware (GPU) or software (CPU) based rendering.
		///
		static bool s_hardwareRendering;

		///
		/// Vertical sync.
		///
		static bool s_vsync;

	private:
		///
		/// Constructor.
		///
		WindowSettings() = delete;
	};
}

#endif