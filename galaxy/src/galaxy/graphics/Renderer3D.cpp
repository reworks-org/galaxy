///
/// Renderer3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Mesh.hpp"
#include "galaxy/graphics/Skybox.hpp"
#include "galaxy/graphics/light/Directional.hpp"
#include "galaxy/graphics/light/Material.hpp"
#include "galaxy/graphics/light/Spot.hpp"
#include "galaxy/graphics/light/Object.hpp"
#include "galaxy/graphics/Shader.hpp"

#include "Renderer3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer3D::~Renderer3D() noexcept
		{
			clean_up();
		}

		Renderer3D& Renderer3D::inst() noexcept
		{
			static Renderer3D s_inst;
			return s_inst;
		}

		void Renderer3D::init(const int width, const int height)
		{
			m_gbuffer.init(width, height);

			// clang-format off
			constexpr const std::array<float, 20> verticies =
			{
				// First 3 are Pos, last 2 are Texels.
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
			};
			// clang-format on

			glGenVertexArrays(1, &m_screen_vao);
			glGenBuffers(1, &m_screen_vbo);
			glBindVertexArray(m_screen_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void Renderer3D::prepare() noexcept
		{
			m_gbuffer.clear_framebuffer();
		}

		void Renderer3D::bind() noexcept
		{
			m_gbuffer.bind();
		}

		void Renderer3D::unbind() noexcept
		{
			m_gbuffer.unbind();
		}

		void Renderer3D::render()
		{
			glBindVertexArray(m_screen_vao);
			const auto& atchmnts = m_gbuffer.get_attachments();

			for (auto index = 0; index < atchmnts.size(); index++)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, atchmnts[index]);
			}

			for (auto& shader : m_render_passes)
			{
				shader.bind();

				shader.set_uniform("u_pos", 0);
				shader.set_uniform("u_diffuse", 1);
				shader.set_uniform("u_specular", 2);
				shader.set_uniform("u_normals", 3);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}

			for (int index = (atchmnts.size() - 1); index > -1; --index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glUseProgram(0);
			glBindVertexArray(0);

			// Copy depth buffer info.
			// clang-format off
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer.get_fbo());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 
				              0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(),
				              GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// clang-format on
		}

		void Renderer3D::add_renderpass(const std::string& vs, const std::string& fs)
		{
			auto& back = m_render_passes.emplace_back(Shader {});
			back.load_raw(vs, fs);
		}

		void Renderer3D::resize(const int width, const int height)
		{
			m_gbuffer.resize(width, height);
		}

		void Renderer3D::reserve_ssbo(const std::size_t index, const unsigned int size)
		{
			bind_ssbo(index);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_buffers[index]);
				glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			unbind_ssbo();
		}

		void Renderer3D::do_forward_render()
		{
			for (const auto& call : m_forward_calls)
			{
				call();
			}

			m_forward_calls.clear();
		}

		void Renderer3D::reserve_ubo(const std::size_t index, const unsigned int size)
		{
			bind_ubo(index);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_buffers[index]);
				glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			unbind_ubo();
		}

		void Renderer3D::bind_ubo(const std::size_t index)
		{
			if ((index + 1) > m_buffers.size())
			{
				unsigned int buff = 0;

				glGenBuffers(1, &buff);
				glBindBuffer(GL_UNIFORM_BUFFER, buff);
				glBindBufferBase(GL_UNIFORM_BUFFER, index, buff);

				m_buffers.resize(index + 1, 0);
				m_buffers[index] = buff;
			}
			else
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_buffers[index]);
			}
		}

		void Renderer3D::unbind_ubo()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void Renderer3D::bind_ssbo(const std::size_t index)
		{
			if ((index + 1) > m_buffers.size())
			{
				unsigned int buff = 0;

				glGenBuffers(1, &buff);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, buff);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buff);

				m_buffers.resize(index + 1, 0);
				m_buffers[index] = buff;
			}
			else
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers[index]);
			}
		}

		void Renderer3D::unbind_ssbo()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void Renderer3D::draw_mesh_deferred(Mesh* mesh, light::Material* material)
		{
			SL_HANDLE.window()->enable_back_cull();

			mesh->bind();

			if (material->m_use_diffuse_texture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->m_diffuse.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.diffuse", 0);
			}

			if (material->m_use_specular_texture)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->m_specular.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.specular", 1);
			}

			if (material->m_use_normal_texture)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, material->m_normal.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.normal", 2);
			}

			m_gbuffer.m_shader.set_uniform("material.shininess", material->m_shininess);
			m_gbuffer.m_shader.set_uniform("material.diffuse_colours", material->m_diffuse_colours);
			m_gbuffer.m_shader.set_uniform("material.specular_colours", material->m_specular_colours);
			m_gbuffer.m_shader.set_uniform("material.use_diffuse_texture", material->m_use_diffuse_texture);
			m_gbuffer.m_shader.set_uniform("material.use_specular_texture", material->m_use_specular_texture);
			m_gbuffer.m_shader.set_uniform("material.use_normal_texture", material->m_use_normal_texture);

			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, nullptr);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Renderer3D::draw(Skybox* skybox, Shader* shader)
		{
			shader->bind();
			skybox->bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		void Renderer3D::draw(light::Object* light, Shader* shader)
		{
			SL_HANDLE.window()->disable_culling();

			shader->bind();
			shader->set_uniform("u_transform", light->get_transform());

			light->bind();
			glDrawArrays(GL_TRIANGLES, 0, light->get_count());
		}

		void Renderer3D::clean_up()
		{
			if (!m_buffers.empty())
			{
				glDeleteBuffers(m_buffers.size(), m_buffers.data());
				m_buffers.clear();
			}

			m_gbuffer.destroy();

			glDeleteVertexArrays(1, &m_screen_vao);
			glDeleteBuffers(1, &m_screen_vbo);

			m_screen_vbo = 0;
			m_screen_vao = 0;
		}

		GeomBuffer& Renderer3D::get_gbuffer() noexcept
		{
			return m_gbuffer;
		}
	} // namespace graphics
} // namespace galaxy