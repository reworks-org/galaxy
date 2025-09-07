///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/resource/embedded/R2DShader.hpp"
#include "galaxy/resource/Shaders.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer& Renderer::ref()
		{
			static Renderer renderer;
			return renderer;
		}

		Renderer::Renderer()
			: m_camera {GAlAXY_BUFFER_CAMERA_INDEX}
			, m_renderdata {GAlAXY_BUFFER_RENDERDATA_INDEX}
		{
		}

		void Renderer::init()
		{
			// Reserve some upfront memory to ease initial allocations.
			if (m_cmds.capacity() < GALAXY_RENDERCOMMAND_INITIAL_ALLOCS)
			{
				m_cmds.reserve(GALAXY_RENDERCOMMAND_INITIAL_ALLOCS);
			}

			if (m_r2d_shader.parse(resource::r2d_vert_shader, resource::r2d_frag_shader))
			{
				m_r2d_shader.compile();
			}

			m_camera.buffer<Camera::Data>(0, 1, nullptr);
			m_renderdata.buffer<RenderData>(0, 1, nullptr);

			auto&      w      = core::ServiceLocator<core::Window>::ref();
			const auto width  = w.frame_width();
			const auto height = w.frame_height();

			m_post.init(width, width);
			m_post.add<graphics::ChromaticAberration>(width, width);
			m_post.add<graphics::FilmicGrain>(width, width);
			m_post.add<graphics::GammaCorrection>(width, width);
			m_post.add<graphics::GaussianBlur>(width, width);
			m_post.add<graphics::Sharpen>(width, width);
			m_post.add<graphics::SMAA>(width, width);

			calc_viewport(width, height);
		}

		void Renderer::destroy()
		{
			m_cmds.clear();
			m_r2d_shader.destroy();
			m_camera.clear();
			m_renderdata.clear();
			m_post.destroy();
		}

		void Renderer::on_window_resized(const events::WindowResized& e)
		{
			calc_viewport(e.width, e.height);
		}

		void Renderer::submit_camera(Camera& camera)
		{
			m_camera.sub_buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit_cmd(RenderCommand& command)
		{
			m_cmds.emplace_back(std::move(command));
		}

		void Renderer::submit_texture(const Texture2D& texture, VertexArray& va, Transform& tf, const int layer, const float opacity)
		{
			auto& cmd = m_cmds.emplace_back(RenderCommand {});

			cmd.count              = va.count();
			cmd.instances          = va.instances();
			cmd.layer              = layer;
			cmd.mode               = GL_TRIANGLES;
			cmd.offset             = va.offset();
			cmd.vao                = va.id();
			cmd.texture            = texture.id();
			cmd.uniforms.colour    = {1.0f, 1.0f, 1.0f, opacity};
			cmd.uniforms.entity    = -1;
			cmd.uniforms.point     = false;
			cmd.uniforms.textured  = true;
			cmd.uniforms.transform = tf.get_transform();

			submit_cmd(cmd);
		}

		void Renderer::submit_text(Text& text, Transform& tf, const int layer)
		{
			auto& cmd = m_cmds.emplace_back(RenderCommand {});

			cmd.count              = text.vao().count();
			cmd.instances          = text.vao().instances();
			cmd.layer              = layer;
			cmd.mode               = GL_TRIANGLES;
			cmd.offset             = text.vao().offset();
			cmd.vao                = text.vao().id();
			cmd.texture            = text.render_texture().texture();
			cmd.uniforms.colour    = text.m_colour.vec4();
			cmd.uniforms.entity    = -1;
			cmd.uniforms.point     = false;
			cmd.uniforms.textured  = true;
			cmd.uniforms.transform = tf.get_transform();

			submit_cmd(cmd);
		}

		void Renderer::submit_shape(Shape* shape, Transform& tf, const int layer)
		{
			auto& cmd = m_cmds.emplace_back(RenderCommand {});

			cmd.count              = shape->vao().count();
			cmd.instances          = shape->vao().instances();
			cmd.layer              = layer;
			cmd.mode               = shape->mode();
			cmd.offset             = shape->vao().offset();
			cmd.vao                = shape->vao().id();
			cmd.texture            = 0;
			cmd.uniforms.colour    = shape->m_colour.vec4();
			cmd.uniforms.entity    = -1;
			cmd.uniforms.point     = shape->mode() == GL_POINTS ? true : false;
			cmd.uniforms.textured  = false;
			cmd.uniforms.transform = tf.get_transform();

			submit_cmd(cmd);
		}

		void Renderer::draw_texture_to_target(RenderTexture& target, Texture* texture, VertexArray& va, Transform& tf)
		{
			glActiveTexture(GL_TEXTURE0);

			target.bind();
			va.bind();
			texture->bind();

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("render_to_texture");
			rtt->bind();
			rtt->set_uniform("u_projection", target.get_proj());
			rtt->set_uniform("u_transform", tf.get_transform());

			glDrawElements(GL_TRIANGLES, va.count(), GL_UNSIGNED_INT, va.offset());

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		void Renderer::draw()
		{
			// I don't think we would ever sort the amount of renderable data required to do this in parallel faster.
			std::sort(m_cmds.begin(), m_cmds.end(), [](auto left, auto right) -> bool {
				return left.layer < right.layer;
			});

			m_r2d_shader.bind();
			m_camera.bind();
			m_renderdata.bind();

			glActiveTexture(GL_TEXTURE0);

			for (auto& cmd : m_cmds)
			{
				m_renderdata.sub_buffer<RenderData>(0, 1, &cmd.uniforms);

				glBindTexture(GL_TEXTURE_2D, cmd.texture);
				glBindVertexArray(cmd.vao);
				glDrawElementsInstanced(cmd.mode, cmd.count, GL_UNSIGNED_INT, cmd.offset, cmd.instances);
			}
		}

		void Renderer::flush()
		{
			m_cmds.clear();
		}

		void Renderer::clear_active()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Renderer::begin_post()
		{
			m_post.bind();
			clear_active();
		}

		void Renderer::end_post()
		{
			m_post.render_effects();
			m_post.unbind();
		}

		void Renderer::render_post()
		{
			m_post.render_output();
		}

		void Renderer::begin_default()
		{
#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
			clear_active();

			// model view i.e. camera.
			// glOrtho(0, screen_width, screen_height, 0, -1, 1);
			// float scale_x = (float)((float)(screen_width)) / (float)virtual_width);
			// float scale_y = (float)((float)(screen_height) / (float)virtual_height);
			// glScalef(scale_x, scale_y, 1.0f);

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
		}

		void Renderer::end_default()
		{
			glfwSwapBuffers(core::ServiceLocator<core::Window>::ref().handle());
		}

		void Renderer::calc_viewport(int window_width, int window_height)
		{
			auto& w = core::ServiceLocator<core::Window>::ref();

			int width  = window_width;
			int height = static_cast<int>(width / w.aspect_ratio() + 0.5f);

			if (height > window_height)
			{
				// It doesn't fit our height, we must switch to pillarbox then.
				height = window_height;
				width  = static_cast<int>(height * w.aspect_ratio() + 0.5f);
			}

			// Set up the new viewport centered in the backbuffer.
			m_viewport.x = static_cast<float>((window_width / 2) - (width / 2));
			m_viewport.y = static_cast<float>((window_height / 2) - (height / 2));
			m_viewport.z = static_cast<float>(width);
			m_viewport.w = static_cast<float>(height);
		}
	} // namespace graphics
} // namespace galaxy
