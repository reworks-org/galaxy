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
#include "galaxy/graphics/shader/Shader.hpp"

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

		void Renderer3D::reserve_ssbo(const unsigned int index, const unsigned int size)
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

		void Renderer3D::reserve_ubo(const unsigned int index, const unsigned int size)
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

		void Renderer3D::bind_ubo(const unsigned int index)
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

		void Renderer3D::bind_ssbo(const unsigned int index)
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

		void Renderer3D::draw_mesh(Mesh* mesh, light::Material* material, Shader* shader)
		{
			SL_HANDLE.window()->enable_back_cull();
			shader->bind();
			mesh->bind();

			if (material->m_use_diffuse_texture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->m_diffuse.gl_texture());
				shader->set_uniform("material.diffuse", 0);
			}

			if (material->m_use_specular_texture)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->m_specular.gl_texture());
				shader->set_uniform("material.specular", 1);
			}

			if (material->m_use_normal_texture)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, material->m_normal.gl_texture());
				shader->set_uniform("material.normal", 2);
			}

			shader->set_uniform("material.shininess", material->m_shininess);
			shader->set_uniform("material.diffuse_colours", material->m_diffuse_colours);
			shader->set_uniform("material.specular_colours", material->m_specular_colours);
			shader->set_uniform("material.use_diffuse_texture", material->m_use_diffuse_texture);
			shader->set_uniform("material.use_specular_texture", material->m_use_specular_texture);
			shader->set_uniform("material.use_normal_texture", material->m_use_normal_texture);

			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer3D::draw_skybox(Skybox* skybox, Shader* shader)
		{
			shader->bind();

			skybox->bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		void Renderer3D::draw_light_object(light::Object* light, Shader* shader)
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
			}
		}
	} // namespace graphics
} // namespace galaxy