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

			m_ctx = nk_glfw3_init(window.handle(), NK_GLFW3_DEFAULT, NK_MAX_VERTEX_BUFFER, NK_MAX_ELEMENT_BUFFER);

			toggle_input(false);
			scale(window.get_content_scale_max());
		}

		NuklearUI::~NuklearUI()
		{
			nk_glfw3_shutdown();
			m_ctx   = nullptr;
			m_atlas = nullptr;
		}

		void NuklearUI::on_mouse_pressed(events::MousePressed& e)
		{
			if (!e.handled)
			{
				nk_glfw3_mouse_button_callback(nullptr, static_cast<int>(e.button), GLFW_PRESS, static_cast<int>(e.mod));
				e.handled = true;
			}
		}

		void NuklearUI::on_mouse_wheel(events::MouseWheel& e)
		{
			if (!e.handled)
			{
				nk_gflw3_scroll_callback(nullptr, e.xoff, e.yoff);
				e.handled = true;
			}
		}

		void NuklearUI::on_key_char(events::KeyChar& e)
		{
			if (!e.handled)
			{
				nk_glfw3_char_callback(nullptr, e.codepoint);
				e.handled = true;
			}
		}

		void NuklearUI::on_key_press(events::KeyPress& e)
		{
			if (!e.handled)
			{
				int action = GLFW_RELEASE;

				if (e.pressed == true)
				{
					action = GLFW_PRESS;
				}
				else if (e.repeat == true)
				{
					action = GLFW_REPEAT;
				}

				nk_glfw3_key_button_callback(nullptr, input::key_to_int(e.keycode), e.scancode, action, input::mod_to_int(e.mod));
				e.handled = true;
			}
		}

		void NuklearUI::on_content_scale(const events::ContentScale& e)
		{
			scale(std::max(e.xscale, e.yscale));
		}

		void NuklearUI::begin_input() const
		{
			if (glfw.do_input)
			{
				nk_input_begin(ctx());
			}
		}

		void NuklearUI::end_input() const
		{
			if (glfw.do_input)
			{
				for (auto i = 0; i < glfw.text_len; ++i)
				{
					nk_input_unicode(&glfw.ctx, glfw.text[i]);
				}

				double x = 0.0, y = 0.0;
				glfwGetCursorPos(glfw.win, &x, &y);
				nk_input_motion(&glfw.ctx, static_cast<int>(x), static_cast<int>(y));

				nk_input_button(&glfw.ctx,
					NK_BUTTON_DOUBLE,
					static_cast<int>(glfw.double_click_pos.x),
					static_cast<int>(glfw.double_click_pos.y),
					glfw.is_double_click_down);
				nk_input_scroll(&glfw.ctx, glfw.scroll);
				nk_input_end(ctx());
				glfw.text_len = 0;
				glfw.scroll   = nk_vec2(0, 0);
			}
		}

		void NuklearUI::poll_input() const
		{
			begin_input();
			glfwPollEvents();
			end_input();
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

		void NuklearUI::toggle_input(const bool enable)
		{
			m_ctx->do_input = enable;

			if (!enable)
			{
				m_ctx->mod_held = false;
			}
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

		nk_context* NuklearUI::ctx() const
		{
			return &m_ctx->ctx;
		}

		void NuklearUI::scale(const float scale)
		{
			auto& fs     = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& config = core::ServiceLocator<core::Config>::ref();

			if (m_atlas)
			{
				nk_font_atlas_clear(m_atlas);
			}

			nk_glfw3_font_stash_begin(&m_atlas);

			for (const auto& file : fs.list(GALAXY_UI_FONTS_DIR))
			{
				auto data = fs.read_binary(file);
				if (!data.empty())
				{
					const auto size = scale * config.get<float>("ui_font_size");

					auto font_cfg       = nk_font_config(size);
					font_cfg.pixel_snap = true;

					auto font = nk_font_atlas_add_from_memory(m_atlas, data.data(), data.size(), size, &font_cfg);
					m_fonts.emplace(file, font);
				}
			}

			nk_glfw3_font_stash_end();
			nk_style_default(ctx());
		}
	} // namespace ui
} // namespace galaxy
