///
/// Mesh.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>
#include <meshoptimizer.h>

#include "Mesh.hpp"

namespace
{
	///
	/// 3D vertex compatible with mesh optimizer.
	///
	struct MeshOptVertex final
	{
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};
} // namespace

namespace galaxy
{
	namespace graphics
	{
		Mesh::~Mesh() noexcept
		{
			m_verticies.clear();
			m_indicies.clear();
		}

		void Mesh::create()
		{
			m_vb.create<graphics::Vertex3D>(m_verticies);
			m_ib.create(m_indicies);

			m_layout.add<graphics::Vertex3D, graphics::VertexAttributes::POSITION>(3);
			m_layout.add<graphics::Vertex3D, graphics::VertexAttributes::NORMAL>(3);
			m_layout.add<graphics::Vertex3D, graphics::VertexAttributes::TEXEL>(2);

			m_va.create<graphics::Vertex3D>(m_vb, m_ib, m_layout);
		}

		void Mesh::optimize()
		{
			static constexpr const auto mesh_vertex_size = sizeof(MeshOptVertex);

			// First convert to the correct vertex format to be optimized.
			std::vector<MeshOptVertex> meshopt_verticies;
			meshopt_verticies.reserve(m_verticies.size());

			for (const auto& vertex : m_verticies)
			{
				MeshOptVertex mov;
				mov.px = vertex.m_pos.x;
				mov.py = vertex.m_pos.y;
				mov.pz = vertex.m_pos.z;

				mov.nx = vertex.m_normal.x;
				mov.ny = vertex.m_normal.y;
				mov.nz = vertex.m_normal.z;

				mov.tx = vertex.m_texels.x;
				mov.ty = vertex.m_texels.y;

				meshopt_verticies.emplace_back(mov);
			}

			// Next generate the remap table for these verticies and indicies.
			const auto index_count = m_indicies.size();
			std::vector<unsigned int> remapped(index_count, 0);
			const auto vertex_count = meshopt_generateVertexRemap(
			    remapped.data(),
			    m_indicies.data(),
			    index_count,
			    meshopt_verticies.data(),
			    meshopt_verticies.size(),
			    mesh_vertex_size);

			// Remap buffers with updated table.
			m_indicies.clear();
			m_indicies.resize(index_count, 0);
			meshopt_remapIndexBuffer(m_indicies.data(), NULL, index_count, remapped.data());

			std::vector<MeshOptVertex> optimized;
			optimized.resize(vertex_count);
			meshopt_remapVertexBuffer(optimized.data(), meshopt_verticies.data(), meshopt_verticies.size(), mesh_vertex_size, remapped.data());

			// Clean up unused memory.
			m_indicies.shrink_to_fit();
			optimized.shrink_to_fit();

			// Perform in-place optimizations on remapped verticies and indicies.
			meshopt_optimizeVertexCache(m_indicies.data(), m_indicies.data(), m_indicies.size(), optimized.size());
			meshopt_optimizeOverdraw(m_indicies.data(), m_indicies.data(), m_indicies.size(), &optimized[0].px, optimized.size(), mesh_vertex_size, 1.01f);
			meshopt_optimizeVertexFetch(optimized.data(), m_indicies.data(), m_indicies.size(), optimized.data(), optimized.size(), mesh_vertex_size);

			// Convert optimized verticies back to glm format.
			m_verticies.clear();
			for (const auto& vertex : optimized)
			{
				Vertex3D glmv3d;
				glmv3d.m_pos.x = vertex.px;
				glmv3d.m_pos.y = vertex.py;
				glmv3d.m_pos.z = vertex.pz;

				glmv3d.m_normal.x = vertex.nx;
				glmv3d.m_normal.y = vertex.ny;
				glmv3d.m_normal.z = vertex.nz;

				glmv3d.m_texels.x = vertex.tx;
				glmv3d.m_texels.y = vertex.ty;

				m_verticies.emplace_back(glmv3d);
			}
			m_verticies.shrink_to_fit();
		}

		void Mesh::draw(Shader* shader)
		{
			unsigned int diffuse_count  = 1;
			unsigned int specular_count = 1;
			unsigned int normal_count   = 1;
			unsigned int height_count   = 1;

			int counter = 0;
			for (const auto& [filename, texture] : m_textures)
			{
				glActiveTexture(GL_TEXTURE0 + counter);

				std::string number;
				switch (texture.m_type)
				{
					case TextureMap::Type::DIFFUSE:
						number = std::to_string(diffuse_count++);
						break;

					case TextureMap::Type::SPECULAR:
						number = std::to_string(specular_count++);
						break;

					case TextureMap::Type::NORMAL:
						number = std::to_string(normal_count++);
						break;

					case TextureMap::Type::HEIGHT:
						number = std::to_string(height_count++);
						break;
				}

				const auto name = "tex_" + static_cast<std::string>(magic_enum::enum_name<TextureMap::Type>(texture.m_type)) + number;

				shader->set_uniform(name, counter);
				glBindTexture(GL_TEXTURE_2D, texture.m_texture.gl_texture());

				counter++;
			}

			shader->set_uniform("material.ambient", m_material.m_ambient);
			shader->set_uniform("material.diffuse", m_material.m_diffuse);
			shader->set_uniform("material.specular", m_material.m_specular);
			shader->set_uniform("material.shininess", m_material.m_shininess);

			m_va.bind();
			glDrawElements(GL_TRIANGLES, m_ib.count(), GL_UNSIGNED_INT, nullptr);

			m_va.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
		}
	} // namespace graphics
} // namespace galaxy