///
/// ImGuiHelpers.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_IMGUIHELPERS_HPP_
#define GALAXY_UI_IMGUIHELPERS_HPP_

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_stdlib.h>

namespace galaxy
{
	namespace ui
	{
		[[maybe_unused]] ImGuiIO& imgui_init_context() noexcept;
		void imgui_new_frame() noexcept;
		void imgui_render() noexcept;
		void imgui_destroy_context() noexcept;
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

/*
  template<meta::is_class Type>
  [[maybe_unused]] const bool im_draw_class(Type& value, const char* label = "");

  template<meta::is_class Type>
  const bool im_draw_class(Type& value, const char* label)
  {
  static_assert("Unsupported class type.");
  return false;
  }

  template<>
  const bool im_draw_class(glm::vec3& value, const char* label)
  {
  int clicked = 0;

  if (std::strlen(label) > 0)
  {
  ImGui::Text(label);
  }

  ImGui::SetNextItemWidth(60.0f);
  clicked += ImGui::InputFloat("X", &value.x, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
  ImGui::SameLine();

  ImGui::SetNextItemWidth(60.0f);
  clicked += ImGui::InputFloat("Y", &value.y, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
  ImGui::SameLine();

  ImGui::SetNextItemWidth(60.0f);
  clicked += ImGui::InputFloat("Z", &value.z, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

  return static_cast<bool>(clicked);
  }
  */

#endif