///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/DefaultShaders.hpp"
#include "galaxy/resource/Shaders.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		std::unique_ptr<UniformBuffer> Renderer::s_camera_ubo       = nullptr;
		std::unique_ptr<UniformBuffer> Renderer::s_r2d_ubo          = nullptr;
		std::unique_ptr<ShaderStorageBuffer> Renderer::s_light_ssbo = nullptr;
		meta::vector<RenderCommand> Renderer::s_data;
		graphics::Shader Renderer::s_r2d_shader;
		int Renderer::s_prev_texture = -1;
		int Renderer::s_prev_nm      = -1;

		void Renderer::init()
		{
			s_camera_ubo = std::make_unique<UniformBuffer>();
			s_r2d_ubo    = std::make_unique<UniformBuffer>();
			s_light_ssbo = std::make_unique<ShaderStorageBuffer>();

			s_camera_ubo->create<Camera::Data>(GAlAXY_UBO_CAMERA_INDEX);
			s_r2d_ubo->create<Render2DUniform>(GAlAXY_UBO_R2D_INDEX);
			s_light_ssbo->create<LightSSBO>(GALAXY_SSBO_LIGHT_INDEX);

			s_data.reserve(GALAXY_DEFAULT_RENDERER_RESERVED);

			s_r2d_shader.load_raw(shaders::r2d_vert, shaders::r2d_frag);
			s_r2d_shader.compile();
		}

		void Renderer::destroy()
		{
			s_data.clear();
			s_camera_ubo.reset();
			s_r2d_ubo.reset();
			s_light_ssbo.reset();
			s_r2d_shader.destroy();

			s_prev_texture = -1;
			s_prev_nm      = -1;
			s_camera_ubo   = nullptr;
			s_r2d_ubo      = nullptr;
			s_light_ssbo   = nullptr;
		}

		void Renderer::buffer_camera(Camera& camera)
		{
			s_camera_ubo->buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::buffer_light_data(LightSSBO& data)
		{
			thread_local const constexpr auto vec2_size  = sizeof(glm::vec2);
			thread_local const constexpr auto vec4_size  = sizeof(glm::vec4);
			thread_local const constexpr auto light_size = sizeof(Light);

			const auto total = vec2_size + vec4_size + (light_size * data.lights.size());

			s_light_ssbo->resize(total);
			s_light_ssbo->buffer<glm::vec4>(0, 1, &data.ambient_light_colour);
			s_light_ssbo->buffer<glm::vec2>(sizeof(glm::vec4), 1, &data.resolution);
			s_light_ssbo->buffer<Light>(sizeof(glm::vec4) + sizeof(glm::vec2), data.lights.size(), data.lights.data());
		}

		void Renderer::submit(RenderCommand& command)
		{
			s_data.emplace_back(std::move(command));
		}

		void Renderer::draw()
		{
			s_prev_texture = -1;
			s_prev_nm      = -1;

			// I don't think we would ever sort the amount of renderable data required to do this in parallel faster.
			// std::execution::par maybe used when > 1000? But 1000 draw calls is too slow anyway.

			std::sort(s_data.begin(), s_data.end(), [](const RenderCommand& left, const RenderCommand& right) -> bool {
				if (left.renderable->get_layer() == right.renderable->get_layer())
				{
					return left.renderable->get_texture_handle() < right.renderable->get_texture_handle();
				}
				else
				{
					return left.renderable->get_layer() < right.renderable->get_layer();
				}
			});

			s_r2d_shader.bind();

			for (auto& cmd : s_data)
			{
				s_r2d_ubo->buffer<Render2DUniform>(0, 1, &cmd.uniform_data);

				glBindVertexArray(cmd.renderable->get_vao().id());

				const auto tex = cmd.renderable->get_texture_handle();
				if (s_prev_texture != tex)
				{
					glBindTextureUnit(GL_TEXTURE0, tex);
					s_prev_texture = tex;
				}

				if (cmd.uniform_data.normal_mapped)
				{
					const auto nm = cmd.normalmap->normal_map_texture();
					if (s_prev_nm != nm)
					{
						glBindTextureUnit(GL_TEXTURE1, nm);
						s_prev_nm = nm;
					}
				}

				// Instances = 1 is the same as glDrawElements.
				glDrawElementsInstanced(cmd.mode, cmd.renderable->get_vao().index_count(), GL_UNSIGNED_INT, nullptr, cmd.instances);
			}
		}

		void Renderer::flush()
		{
			s_data.clear();
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

		void Renderer::draw_texture_to_framebuffer(const unsigned int texture, VertexArray& va, components::Transform& transform, const glm::mat4& proj)
		{
			va.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("RenderToTexture");
			if (rtt != nullptr)
			{
				rtt->bind();
				rtt->set_uniform("u_projection", proj);
				rtt->set_uniform("u_transform", transform.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default draw_texture_to_framebuffer shader.");
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	} // namespace graphics
} // namespace galaxy