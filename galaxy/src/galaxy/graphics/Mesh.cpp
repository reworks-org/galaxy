///
/// Mesh.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "Mesh.hpp"

namespace galaxy
{
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
			m_layout.add<graphics::Vertex3D, graphics::VertexAttributes::TANGENTS>(3);
			m_layout.add<graphics::Vertex3D, graphics::VertexAttributes::TEXEL>(2);

			m_va.create<graphics::Vertex3D>(m_vb, m_ib, m_layout);
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