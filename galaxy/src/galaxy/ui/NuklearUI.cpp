///
/// NuklearUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define NK_GLFW_GL4_IMPLEMENTATION

#include <entt/entity/registry.hpp>

#include "galaxy/components/Flag.hpp"
#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

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

			auto dir      = config.get<std::string>("ui_folder", "resource_folders") + "fonts/";
			auto contents = core::ServiceLocator<fs::VirtualFileSystem>::ref().list_directory(dir);
			for (std::filesystem::path file : contents)
			{
				m_fonts.emplace(file.stem().string(), nk_font_atlas_add_from_file(m_atlas, file.string().c_str(), 14, nullptr));
			}

			nk_glfw3_font_stash_end();
			nk_style_default(&m_ctx->ctx);
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

		void NuklearUI::process_scripts(entt::registry& registry)
		{
			const auto view = registry.view<components::UIScript, components::Flag>();
			for (auto&& [entity, script, flag] : view.each())
			{
				if (flag.is_flag_set<flags::Enabled>() && script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
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
				nk_style_set_font(&m_ctx->ctx, &m_fonts[id]->handle);
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

		nk_context* NuklearUI::ctx() const
		{
			return &m_ctx->ctx;
		}
	} // namespace ui
} // namespace galaxy