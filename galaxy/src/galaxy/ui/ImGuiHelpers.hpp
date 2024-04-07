///
/// ImGuiHelpers.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_IMGUIHELPERS_HPP_
#define GALAXY_UI_IMGUIHELPERS_HPP_

#include <functional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui/imgui_stdlib.h>

namespace galaxy
{
	namespace graphics
	{
		class Texture2D;
		class RenderTexture;
	} // namespace graphics

	namespace ui
	{
		///
		/// Initialize ImGui context with galaxy.
		///
		/// \return Reference to initialized imgui io.
		///
		[[maybe_unused]] ImGuiIO& imgui_init_context();

		///
		/// New imgui frame with galaxy.
		///
		void imgui_new_frame();

		///
		/// Render imgui with galaxy.
		///
		void imgui_render();

		///
		/// Cleanup imgui resources used by galaxy.
		///
		void imgui_destroy_context();

		///
		/// Open a confirmation popup.
		///
		/// \param popup String ID of popup to open.
		///
		void imgui_open_confirm(const char* popup);

		///
		/// Render the confirmation popup.
		///
		/// \param popup String ID of popup to open.
		/// \param yes Callback if user selects the yes option.
		/// \param no Callback if user selects the no option.
		///
		void imgui_confirm(const char* popup, const std::function<void(void)>& yes = {}, const std::function<void(void)>& no = {});

		///
		///
		///

		/*
		ImGui::Begin("Manual Row background Test");
		ImGui::Text("Some text before..");

		float x1 = ImGui::GetCurrentWindow()->WorkRect.Min.x;
		float x2 = ImGui::GetCurrentWindow()->WorkRect.Max.x;
		float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
		float item_offset_y = -item_spacing_y * 0.5f;
		float line_height = ImGui::GetTextLineHeight() + item_spacing_y;
		DrawRowsBackground(50, line_height, x1, x2, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, 0.5f)));

		for (int n = 0; n < 50; n++)
			ImGui::Text("Item %03d", n);

		ImGui::End();
*/
		void draw_rows_background(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd);

		///
		/// Make sure the next window opened is centered on the monitor.
		///
		void imgui_center_next_window();

		///
		/// Render an imgui image button with a galaxy texture.
		///
		/// \param texture Texture to display.
		/// \param size Size of the button.
		/// \param bg_col Optional background colour.
		/// \param tint_col Optional tint colour.
		///
		/// \return True if button pressed.
		///
		[[maybe_unused]] bool imgui_imagebutton(const graphics::Texture2D& texture,
			const ImVec2&                                                  size,
			const ImVec4&                                                  bg_col   = ImVec4(0, 0, 0, 0),
			const ImVec4&                                                  tint_col = ImVec4(1, 1, 1, 1));

		///
		/// Display a galaxy image with imgui.
		///
		/// \param texture Texture to display.
		/// \param size Size of the button.
		///
		void imgui_image(const graphics::Texture2D& texture, const ImVec2& size);

		///
		/// Display a galaxy image with imgui.
		///
		/// \param texture Render texture to display.
		/// \param size Size of the button.
		///
		void imgui_image(const graphics::RenderTexture& texture, const ImVec2& size);

		///
		/// ImGui success notification.
		///
		/// \param msg Message to display.
		///
		void imgui_notify_success(const char* msg);

		///
		/// ImGui info notification.
		///
		/// \param msg Message to display.
		///
		void imgui_notify_info(const char* msg);

		///
		/// ImGui warning notification.
		///
		/// \param msg Message to display.
		///
		void imgui_notify_warning(const char* msg);

		///
		/// ImGui error notification.
		///
		/// \param msg Message to display.
		///
		void imgui_notify_error(const char* msg);

		///
		/// GLM vector 2 widget.
		///
		/// \param label Widget label.
		/// \param vec Vector.
		///
		/// \return True if any field is clicked.
		///
		[[maybe_unused]] bool imgui_glm_vec2(const char* label, glm::vec2& vec);

		///
		/// GLM vector 3 widget.
		///
		/// \param label Widget label.
		/// \param vec Vector.
		///
		/// \return True if any field is clicked.
		///
		[[maybe_unused]] bool imgui_glm_vec3(const char* label, glm::vec3& vec);

		///
		/// GLM vector 4 widget.
		///
		/// \param label Widget label.
		/// \param vec Vector.
		///
		/// \return True if any field is clicked.
		///
		[[maybe_unused]] bool imgui_glm_vec4(const char* label, glm::vec4& vec);
	} // namespace ui
} // namespace galaxy

#endif
