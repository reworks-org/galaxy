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

		void Renderer::init()
		{
			// Reserve some upfront memory to ease initial allocations.
			if (m_cmds.capacity() < 10)
			{
				m_cmds.reserve(10);
			}

			if (m_r2d_shader.parse(resource::r2d_vert_shader, resource::r2d_frag_shader))
			{
				m_r2d_shader.compile();
			}

			m_camera.buffer<Camera::Data>(GAlAXY_BUFFER_CAMERA_INDEX, 0, 1, nullptr);
			m_renderdata.buffer<RenderData>(GAlAXY_BUFFER_RENDERDATA_INDEX, 0, 1, nullptr);

			auto& window = core::ServiceLocator<core::Window>::ref();
			m_width      = window.get_width();
			m_height     = window.get_height();

			m_post.init(m_width, m_height);
			m_post.add<graphics::ChromaticAberration>(m_width, m_height);
			m_post.add<graphics::FilmicGrain>(m_width, m_height);
			m_post.add<graphics::GammaCorrection>(m_width, m_height);
			m_post.add<graphics::GaussianBlur>(m_width, m_height);
			m_post.add<graphics::Sharpen>(m_width, m_height);
			m_post.add<graphics::SMAA>(m_width, m_height);
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
			m_width  = e.width;
			m_height = e.height;

			m_post.resize(m_width, m_height);
		}

		void Renderer::submit_camera(Camera& camera)
		{
			m_camera.sub_buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit_cmd(RenderCommand& command)
		{
			m_cmds.push_back(&command);
		}

		void Renderer::submit_texture(const Texture2D& texture, VertexArray& va, Transform& tf, const int layer, const float opacity)
		{
			auto& cmd = m_free_cmds.emplace_back(RenderCommand {});

			cmd.count              = va.count();
			cmd.instances          = va.instances();
			cmd.layer              = layer;
			cmd.mode               = GL_TRIANGLES;
			cmd.offset             = va.offset();
			cmd.vao                = va.id();
			cmd.uniforms.colour    = {1.0f, 1.0f, 1.0f, opacity};
			cmd.uniforms.entity    = -1;
			cmd.uniforms.handle    = texture.handle();
			cmd.uniforms.point     = false;
			cmd.uniforms.textured  = true;
			cmd.uniforms.transform = tf.get_transform();

			submit_cmd(cmd);
		}

		void Renderer::submit_text(Text& text, Transform& tf, const int layer)
		{
			auto& cmd = m_free_cmds.emplace_back(RenderCommand {});

			cmd.count              = text.vao().count();
			cmd.instances          = text.vao().instances();
			cmd.layer              = layer;
			cmd.mode               = GL_TRIANGLES;
			cmd.offset             = text.vao().offset();
			cmd.vao                = text.vao().id();
			cmd.uniforms.colour    = text.m_colour.vec4();
			cmd.uniforms.entity    = -1;
			cmd.uniforms.handle    = text.render_texture().handle();
			cmd.uniforms.point     = false;
			cmd.uniforms.textured  = true;
			cmd.uniforms.transform = tf.get_transform();

			submit_cmd(cmd);
		}

		void Renderer::submit_shape(Shape* shape, Transform& tf, const int layer)
		{
			auto& cmd = m_free_cmds.emplace_back(RenderCommand {});

			cmd.count              = shape->vao().count();
			cmd.instances          = shape->vao().instances();
			cmd.layer              = layer;
			cmd.mode               = shape->mode();
			cmd.offset             = shape->vao().offset();
			cmd.vao                = shape->vao().id();
			cmd.uniforms.colour    = shape->m_colour.vec4();
			cmd.uniforms.entity    = -1;
			cmd.uniforms.handle    = 0;
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

			// Use a standard projection.
			const auto proj = glm::ortho(0.0f, static_cast<float>(target.width()), static_cast<float>(target.height()), 0.0f, -1.0f, 1.0f);

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("render_to_texture");
			rtt->bind();
			rtt->set_uniform("u_projection", proj);
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
				return left->layer < right->layer;
			});

			m_r2d_shader.bind();

			for (auto& cmd : m_cmds)
			{
				m_renderdata.sub_buffer<RenderData>(0, 1, &cmd->uniforms);

				glBindVertexArray(cmd->vao);
				glDrawElementsInstanced(cmd->mode, cmd->count, GL_UNSIGNED_INT, cmd->offset, cmd->instances);
			}
		}

		void Renderer::flush()
		{
			m_cmds.clear();
			m_free_cmds.clear();
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

			// glBindVertexArray(0);
			// glBindTexture(GL_TEXTURE_2D, 0);
			// glUseProgram(0);
		}

		void Renderer::begin_default()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);
			clear_active();
		}

		void Renderer::end_default()
		{
			glfwSwapBuffers(core::ServiceLocator<core::Window>::ref().handle());
		}
	} // namespace graphics
} // namespace galaxy
