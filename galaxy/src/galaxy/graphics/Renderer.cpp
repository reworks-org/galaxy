///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/DefaultShaders.hpp"
#include "galaxy/resource/Shaders.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer::Renderer()
			: m_camera_ubo {nullptr}
			, m_r2d_ubo {nullptr}
			, m_prev_texture {-1}
			, m_prev_nm {-1}
			, m_width {1}
			, m_height {1}
		{
			m_camera_ubo = std::make_unique<UniformBuffer>();
			m_r2d_ubo    = std::make_unique<UniformBuffer>();

			m_camera_ubo->create<Camera::Data>(GAlAXY_UBO_CAMERA_INDEX);
			m_r2d_ubo->create<Render2DUniform>(GAlAXY_UBO_R2D_INDEX);

			m_r2d_shader.load_raw(shaders::r2d_vert, shaders::r2d_frag);
			m_r2d_shader.compile();

			auto& window = core::ServiceLocator<core::Window>::ref();
			m_width      = window.get_width();
			m_height     = window.get_height();

			m_postprocess.init(m_width, m_height);
			m_postprocess.add<graphics::ChromaticAberration>(m_width, m_height);
			m_postprocess.add<graphics::GammaCorrection>(m_width, m_height);
			m_postprocess.add<graphics::GaussianBlur>(m_width, m_height);
			m_postprocess.add<graphics::Sharpen>(m_width, m_height);
			m_postprocess.add<graphics::SMAA>(m_width, m_height);
		}

		Renderer::~Renderer()
		{
			m_postprocess.destroy();
			m_r2d_shader.destroy();
		}

		void Renderer::buffer_camera(Camera& camera)
		{
			m_camera_ubo->buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit(RenderCommand& command)
		{
			m_data.emplace_back(std::move(command));
		}

		void Renderer::draw()
		{
			m_prev_texture = -1;
			m_prev_nm      = -1;

			// I don't think we would ever sort the amount of renderable data required to do this in parallel faster.
			// std::execution::par maybe used when > 1000? But 1000 draw calls is too slow anyway.

			std::sort(m_data.begin(), m_data.end(), [](const RenderCommand& left, const RenderCommand& right) -> bool {
				if (left.renderable->get_layer() == right.renderable->get_layer())
				{
					return left.renderable->get_texture_handle() < right.renderable->get_texture_handle();
				}
				else
				{
					return left.renderable->get_layer() < right.renderable->get_layer();
				}
			});

			m_r2d_shader.bind();

			for (auto& cmd : m_data)
			{
				m_r2d_ubo->buffer<Render2DUniform>(0, 1, &cmd.uniform_data);

				glBindVertexArray(cmd.renderable->get_vao().id());

				const auto tex = cmd.renderable->get_texture_handle();
				if (m_prev_texture != tex)
				{
					glBindTextureUnit(0, tex);
					m_prev_texture = tex;
				}

				// Instances = 1 is the same as glDrawElements.
				glDrawElementsInstanced(cmd.mode, cmd.renderable->get_vao().index_count(), GL_UNSIGNED_INT, nullptr, cmd.instances);
			}
		}

		void Renderer::flush()
		{
			m_data.clear();
		}

		void Renderer::begin_postprocessing()
		{
			m_postprocess.bind();
		}

		void Renderer::end_postprocessing()
		{
			m_postprocess.render_effects();
		}

		void Renderer::render_postprocessing()
		{
			m_postprocess.render_output();

			// glBindVertexArray(0);
			// glBindTexture(GL_TEXTURE_2D, 0);
			// glUseProgram(0);
		}

		void Renderer::prepare_default()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);
		}

		void Renderer::clear()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Renderer::swap_buffers(GLFWwindow* window)
		{
			glfwSwapBuffers(window);
		}

		void Renderer::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			m_postprocess.resize(m_width, m_height);
		}

		void Renderer::draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform)
		{
			glActiveTexture(GL_TEXTURE0);

			va.bind();
			texture.bind();

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("RenderToTexture");
			if (rtt != nullptr)
			{
				rtt->bind();
				rtt->set_uniform("u_projection", target.get_proj());
				rtt->set_uniform("u_transform", transform.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default draw_texture_to_target shader.");
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		void Renderer::draw_texture(const unsigned int texture, const int width, const int height)
		{
			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("RenderToTexture");
			if (rtt != nullptr)
			{
				VertexArray va;
				components::Transform tf;

				const auto proj = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

				auto v = graphics::Vertex::gen_quad_vertices(width, height);
				va.create(v, graphics::StorageFlag::STATIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
				va.bind();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);

				rtt->bind();
				rtt->set_uniform("u_projection", proj);
				rtt->set_uniform("u_transform", tf.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
				glUseProgram(0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default draw_texture_to_framebuffer shader.");
			}
		}
	} // namespace graphics
} // namespace galaxy