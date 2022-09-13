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
	namespace ui
	{
		[[maybe_unused]] ImGuiIO& imgui_init_context() noexcept;
		void imgui_new_frame() noexcept;
		void imgui_prerender() noexcept;
		void imgui_render() noexcept;
		void imgui_destroy_context() noexcept;

		void imgui_open_alert(const char* popup) noexcept;
		void imgui_alert(const char* popup, const char* msg) noexcept;

		void imgui_open_confirm(const char* popup) noexcept;
		void imgui_confirm(const char* popup, const std::function<void(void)>& yes = {}, const std::function<void(void)>& no = {}) noexcept;

		void imgui_center_next_window() noexcept;
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