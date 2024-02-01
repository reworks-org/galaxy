///
/// NuklearUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define NK_GLFW_GL4_IMPLEMENTATION

#include <entt/entity/registry.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "NuklearUI.hpp"

namespace galaxy
{
	namespace ui
	{
		NuklearUI::NuklearUI()
			: m_ctx {nullptr}
			, m_atlas {nullptr}
			, m_viewport_backup {0, 0, 0, 0}
		{
			auto& window = core::ServiceLocator<core::Window>::ref();
			auto& config = core::ServiceLocator<core::Config>::ref();

			m_ctx = nk_glfw3_init(window.handle(), NK_GLFW3_DEFAULT, NK_MAX_VERTEX_BUFFER, NK_MAX_ELEMENT_BUFFER);

			nk_glfw3_font_stash_begin(&m_atlas);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			for (const auto& file : fs.list(GALAXY_UI_FONTS_DIR))
			{
				auto data = fs.read_binary(file);
				if (!data.empty())
				{
					auto font = nk_font_atlas_add_from_memory(m_atlas, data.data(), data.size(), config.get<float>("ui_font_size"), nullptr);
					m_fonts.emplace(file, font);
				}
			}

			nk_glfw3_font_stash_end();
			nk_style_default(ctx());
		}

		NuklearUI::~NuklearUI()
		{
			nk_glfw3_shutdown();
			m_ctx   = nullptr;
			m_atlas = nullptr;
		}

		void NuklearUI::on_mouse_pressed(GLFWwindow* window, events::MousePressed& e)
		{
			nk_glfw3_mouse_button_callback(window, static_cast<int>(e.button), GLFW_PRESS, static_cast<int>(e.mod));
			e.handled = true;
		}

		void NuklearUI::on_mouse_wheel(GLFWwindow* window, events::MouseWheel& e)
		{
			nk_gflw3_scroll_callback(window, e.xoff, e.yoff);
			e.handled = true;
		}

		void NuklearUI::on_key_char(GLFWwindow* window, events::KeyChar& e)
		{
			nk_glfw3_char_callback(window, e.codepoint);
			e.handled = true;
		}

		void NuklearUI::new_frame()
		{
			nk_glfw3_new_frame();
		}

		void NuklearUI::render()
		{
			glGetIntegerv(GL_VIEWPORT, m_viewport_backup.data());

			nk_glfw3_render(NK_ANTI_ALIASING_ON);

			glUseProgram(0);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_SCISSOR_TEST);
			glEnable(GL_DEPTH_TEST);

			glViewport(m_viewport_backup[0], m_viewport_backup[1], m_viewport_backup[2], m_viewport_backup[3]);
		}

		void NuklearUI::set_font(const std::string& id)
		{
			if (m_fonts.contains(id))
			{
				nk_style_set_font(ctx(), &m_fonts[id]->handle);
			}
		}

		void NuklearUI::enable_input()
		{
			m_ctx->is_input_allowed = true;
		}

		void NuklearUI::disable_input()
		{
			m_ctx->is_input_allowed = false;
		}

		void NuklearUI::show_loading_bar(const char* text, nk_size total, nk_size current)
		{
			auto& window = core::ServiceLocator<core::Window>::ref();

			if (nk_begin(ctx(),
					"loading_window",
					nk_rect((window.get_widthf() / 2.0f) - 200, (window.get_heightf() / 2.0f) - 30, 400, 60),
					NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
			{
				nk_layout_row_dynamic(ctx(), 20, 1);
				nk_label(ctx(), text, NK_TEXT_CENTERED);
				nk_layout_row_static(ctx(), 20, 400, 1);
				nk_progress(ctx(), &current, total, NK_FIXED);
			}

			nk_end(ctx());
		}

		void NuklearUI::show_building_atlas()
		{
			auto& window = core::ServiceLocator<core::Window>::ref();

			if (nk_begin(ctx(),
					"building_window",
					nk_rect((window.get_widthf() / 2.0f) - 200, (window.get_heightf() / 2.0f) - 20, 400, 40),
					NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
			{
				nk_layout_row_dynamic(ctx(), 20, 1);
				nk_label(ctx(), "Building Textures...", NK_TEXT_CENTERED);
			}

			nk_end(ctx());
		}

		void NuklearUI::show_loading_maps(const int cur, const int total)
		{
			auto& window = core::ServiceLocator<core::Window>::ref();

			if (nk_begin(ctx(),
					"building_window",
					nk_rect((window.get_widthf() / 2.0f) - 200, (window.get_heightf() / 2.0f) - 20, 400, 40),
					NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
			{
				const auto text = std::format("Loading maps... [{0}/{1}]", cur, total);
				nk_layout_row_dynamic(ctx(), 20, 1);
				nk_label(ctx(), text.c_str(), NK_TEXT_CENTERED);
			}

			nk_end(ctx());
		}

		nk_context* NuklearUI::ctx() const
		{
			return &m_ctx->ctx;
		}
	} // namespace ui
} // namespace galaxy
