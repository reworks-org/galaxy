///
/// WindowSettings.hpp
/// quasar
///
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
		/// Destructor.
		///
		~WindowSettings() noexcept = default;

		///
		/// Level of MSAA. I.e. 2, 4, etc.
		///
		static int s_anti_aliasing;

		///
		/// Level of ansiotropic filtering.
		///
		static int s_ansio_filtering;

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
		static int s_aspect_ratio_x;

		///
		/// Aspect Ratio y.
		/// Set to -1 to ignore.
		///
		static int s_aspect_ratio_y;

		///
		/// Raw mouse input.
		///
		static bool s_raw_mouse_input;

		///
		/// Texture format.
		///
		static unsigned int s_texture_format;

	private:
		///
		/// Default constructor.
		///
		WindowSettings() = delete;

		///
		/// Copy constructor.
		///
		WindowSettings(const WindowSettings&) = delete;

		///
		/// Move constructor.
		///
		WindowSettings(WindowSettings&&) = delete;
	};
} // namespace qs

#endif