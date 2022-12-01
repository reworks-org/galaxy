///
/// ImGuiHelpers.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_IMGUIHELPERS_HPP_
#define GALAXY_UI_IMGUIHELPERS_HPP_

#include <functional>

#include <imgui_stdlib.h>

namespace galaxy
{
	namespace graphics
	{
		class Texture;
		class RenderTexture;
	} // namespace graphics

	namespace ui
	{
		///
		/// Initialize ImGui context with galaxy.
		///
		/// \return Reference to initialized imgui io.
		///
		[[maybe_unused]] ImGuiIO& imgui_init_context() noexcept;

		///
		/// New imgui frame with galaxy.
		///
		void imgui_new_frame() noexcept;

		///
		/// Imgui setup to prep for rendering.
		///
		void imgui_prerender() noexcept;

		///
		/// Render imgui with galaxy.
		///
		void imgui_render() noexcept;

		///
		/// Cleanup imgui resources used by galaxy.
		///
		void imgui_destroy_context() noexcept;

		///
		/// Open a confirmation popup.
		///
		/// \param popup String ID of popup to open.
		///
		void imgui_open_confirm(const char* popup) noexcept;

		///
		/// Render the confirmation popup.
		///
		/// \param popup String ID of popup to open.
		/// \param yes Callback if user selects the yes option.
		/// \param no Callback if user selects the no option.
		///
		void imgui_confirm(const char* popup, const std::function<void(void)>& yes = {}, const std::function<void(void)>& no = {}) noexcept;

		///
		/// Make sure the next window opened is centered on the monitor.
		///
		void imgui_center_next_window() noexcept;

		///
		/// Trigger imgui actions from keyboard shortcut.
		///
		/// \param mods Key input modifiers.
		/// \param key Key pressed.
		/// \param flags Focusing flags. Defaults to any window.
		///
		/// \return True if shortcut was pressed.
		///
		[[nodiscard]] bool imgui_shortcut(ImGuiKeyChord mods, ImGuiKey key, ImGuiFocusedFlags flag = ImGuiFocusedFlags_AnyWindow) noexcept;

		///
		/// Render an imgui image button with a galaxy texture.
		///
		/// \param texture Texture to display.
		/// \param size Size of the button.
		///
		/// \return True if button pressed.
		///
		[[maybe_unused]] bool imgui_imagebutton(const graphics::Texture& texture, const ImVec2& size) noexcept;

		///
		/// Display a galaxy image with imgui.
		///
		/// \param texture Texture to display.
		/// \param size Size of the button.
		///
		void imgui_image(const graphics::Texture& texture, const ImVec2& size) noexcept;

		///
		/// Display a galaxy image with imgui.
		///
		/// \param texture Render texture to display.
		/// \param size Size of the button.
		///
		void imgui_image(const graphics::RenderTexture& texture, const ImVec2& size) noexcept;
	} // namespace ui
} // namespace galaxy

[[nodiscard]] inline const bool operator==(const ImVec2& a, const ImVec2& b) noexcept
{
	return ((a.x == b.x) && (a.y == b.y));
}

[[nodiscard]] inline const bool operator!=(const ImVec2& a, const ImVec2& b) noexcept
{
	return !(a == b);
}

#endif