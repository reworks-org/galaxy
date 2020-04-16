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
		/// Level of MSAA. I.e. 2, 4, etc.
		///
		static int s_antiAliasing;

		///
		/// Level of ansiotropic filtering.
		///
		static int s_ansiotropicFiltering;

		///
		/// Vertical sync.
		///
		static bool s_vsync;

		///
		/// sRGB support.
		///
		static bool s_srgb;

		///
		/// Aspect Ratio x.
		/// Set to -1 to ignore.
		///
		static int s_aspectRatioX;

		///
		/// Aspect Ratio y.
		/// Set to -1 to ignore.
		///
		static int s_aspectRatioY;

		///
		/// Raw mouse input.
		///
		static bool s_rawMouseInput;

	private:
		///
		/// Constructor.
		///
		WindowSettings() = delete;
	};
}

#endif