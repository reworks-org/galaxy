///
/// ImGuiTheme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_IMGUITHEME_HPP_
#define GALAXY_UI_IMGUITHEME_HPP_

namespace galaxy
{
	namespace ui
	{
		///
		/// Seralizable identifiers for imgui themes.
		///
		enum class ImguiThemeId : int
		{
			LIGHT,
			DARK,
			CLASSIC,
			ENHANCED_LIGHT,
			MATERIAL_DARK,
			VISUAL_DARK,
			FANCY_DARK,
			DARK_EMBRACE
		};

		///
		/// An enhanced light theme reminiscent of OS X.
		///
		void imgui_theme_enhanced_light() noexcept;

		///
		/// A classic material dark theme.
		///
		void imgui_theme_material_dark() noexcept;

		///
		/// Similar to the default Visual Studio dark theme.
		///
		void imgui_theme_visual_dark() noexcept;

		///
		/// "Fancy Dark" theme.
		///
		void imgui_theme_fancy_dark() noexcept;

		///
		/// "Dark Embrace" theme.
		///
		void imgui_theme_dark_embrace() noexcept;
	} // namespace ui
} // namespace galaxy

#endif