///
/// NuklearUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_NUKLEARUI_HPP_
#define GALAXY_UI_NUKLEARUI_HPP_

#include <Nuklear.hpp>
#include <robin_hood.h>

#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseWheel.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// Manages the nuklear ui state.
		///
		class NuklearUI final
		{
		  public:
			///
			/// Constructor.
			///
			NuklearUI();

			///
			/// Destructor.
			///
			~NuklearUI();

			///
			/// On mouse pressed event handler.
			///
			/// \param window Pointer to GLFW window handle.
			/// \param e Event to process.
			///
			void on_mouse_pressed(GLFWwindow* window, events::MousePressed& e);

			///
			/// On mouse wheel event handler.
			///
			/// \param window Pointer to GLFW window handle.
			/// \param e Event to process.
			///
			void on_mouse_wheel(GLFWwindow* window, events::MouseWheel& e);

			///
			/// On typing event handler.
			///
			/// \param window Pointer to GLFW window handle.
			/// \param e Event to process.
			///
			void on_key_char(GLFWwindow* window, events::KeyChar& e);

			///
			/// Called before any nuklear code.
			///
			void new_frame();

			///
			/// Render nuklear.
			///
			void render();

			///
			/// Change the active font.
			///
			/// \param id Font file name without extension.
			///
			void set_font(const std::string& id);

			///
			/// Enables nuklear input.
			///
			void enable_input();

			///
			/// Disables nuklear input.
			///
			void disable_input();

			///
			/// Shows a progress bar for loading screens.
			///
			/// \param text Message to display when loading.
			/// \param total Total size of loading bar.
			/// \param current Current progress of loading bar.
			///
			void show_loading_bar(const char* text, nk_size total, nk_size current);

			///
			/// Part of the loading screen. Informs the user that the atlas is being built.
			///
			void show_building_atlas();

			///
			/// Loading screen UI for maps.
			///
			/// \param cur Current map being loaded.
			/// \param total Total maps to load.
			///
			void show_loading_maps(const int cur, const int total);

			///
			/// Nuklear Context.
			///
			/// \return Pointer to context. DO NOT FREE.
			///
			[[nodiscard]] nk_context* ctx() const;

		  private:
			///
			/// Nuklear context pointer.
			///
			nk_glfw* m_ctx;

			///
			/// Nuklear atlas pointer.
			///
			nk_font_atlas* m_atlas;

			///
			/// Viewport backup when rendering.
			///
			std::array<int, 4> m_viewport_backup;

			///
			/// Fonts used by nuklear.
			///
			robin_hood::unordered_flat_map<std::string, nk_font*> m_fonts;
		};
	} // namespace ui
} // namespace galaxy

#endif
