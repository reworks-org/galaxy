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

#include "galaxy/math/Rect.hpp"

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
		/// \param ini Layout ini file. Can include a path.
		///
		/// \return Reference to initialized imgui io.
		///
		[[maybe_unused]] ImGuiIO& imgui_init_context(const char* ini);

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
		/// Set our theme.
		///
		void imgui_set_theme();

		///
		/// Cleaner way of rendering an ImGui popup.
		///
		/// \param id Popup ID.
		/// \param open Controls popup visibility.
		/// \param func ImGui controls to show on popup.
		///
		void imgui_popup(const char* id, bool& open, std::move_only_function<void(void)>&& func);

		///
		/// Handles a popup for a yes/no situation.
		///
		/// \param msg Text to display on confirmation box.
		/// \param yes Code to run if user selects yes.
		/// \param no Code to run if user selects no.
		///
		void imgui_confirm(const char* msg, std::move_only_function<void(void)>&& yes = {}, std::move_only_function<void(void)>&& no = {});

		///
		/// Make sure the next window opened is centered on the monitor.
		///
		void imgui_center_next_window();

		///
		/// \brief Scale and load fonts.
		///
		/// Calc proper window scalings and needs to reload fonts aswell.
		///
		void scale_and_load_fonts();

		///
		/// Is ImGui loaded?
		///
		/// \return True if imgui context is loaded.
		///
		[[nodiscard]] bool imgui_loaded();

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

		///
		/// fRect widget.
		///
		/// \param label Widget label.
		/// \param rect Rectangle.
		///
		/// \return True if any field is clicked.
		///
		[[maybe_unused]] bool imgui_frect(const char* label, math::fRect& rect);
	} // namespace ui
} // namespace galaxy

#endif
