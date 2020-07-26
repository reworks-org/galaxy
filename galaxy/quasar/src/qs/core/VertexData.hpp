///
/// VertexData.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXDATA_HPP_
#define QUASAR_VERTEXDATA_HPP_

#include "qs/vertex/VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Provides a class with a method of rendering to screen.
	///
	class VertexData
	{
	public:
		///
		/// Constructor.
		///
		VertexData() noexcept;

		///
		/// Virtual destructor.
		///
		virtual ~VertexData() noexcept = default;

		///
		/// Get z-level of vertexs.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int z_level() const noexcept;

		///
		/// Get VBO.
		///
		/// \return Reference to VBO.
		///
		qs::VertexBuffer& get_vbo() noexcept;

		///
		/// Get VAO.
		///
		/// \return Reference to VAO.
		///
		qs::VertexArray& get_vao() noexcept;

		///
		/// Get index count.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int index_count() const noexcept;

	protected:
		///
		/// Z-Level
		///
		unsigned int m_z_level;

		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_va;

		///
		/// Vertex buffer.
		///
		qs::VertexBuffer m_vb;

		///
		/// Index (Element) buffer.
		///
		qs::IndexBuffer m_ib;

		///
		/// Vertex layout.
		///
		qs::VertexLayout m_layout;
	};
} // namespace qs

#endif