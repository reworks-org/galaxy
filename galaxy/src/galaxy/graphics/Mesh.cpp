///
/// Mesh.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>
#include <meshoptimizer.h>

#include "Mesh.hpp"

namespace galaxy
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

	namespace graphics
	{
		Mesh::Mesh(Mesh&& m) noexcept
		    : VertexData {std::move(m)}
		{
			this->m_verticies   = std::move(m.m_verticies);
			this->m_indicies    = std::move(m.m_indicies);
			this->m_material_id = std::move(m.m_material_id);
		}

		Mesh& Mesh::operator=(Mesh&& m) noexcept
		{
			if (this != &m)
			{
				VertexData::operator=(std::move(m));

				this->m_verticies   = std::move(m.m_verticies);
				this->m_indicies    = std::move(m.m_indicies);
				this->m_material_id = std::move(m.m_material_id);
			}

			return *this;
		}

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

		void Mesh::bind() noexcept
		{
			m_va.bind();
		}

		void Mesh::unbind() noexcept
		{
			m_va.unbind();
		}

		const unsigned int Mesh::index_count() const noexcept
		{
			return m_ib.count();
		}
	} // namespace graphics
} // namespace galaxy