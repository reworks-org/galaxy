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
		virtual const unsigned int getZLevel() const noexcept final;

		///
		/// Get VBO.
		///
		/// \return Reference to VBO.
		///
		virtual qs::VertexBuffer& getVBO() noexcept final;

		///
		/// Get VAO.
		///
		/// \return Reference to VAO.
		///
		virtual qs::VertexArray& getVAO() noexcept final;

		///
		/// Get index count.
		///
		/// \return Const unsigned integer.
		///
		virtual const unsigned int getCount() const noexcept;

	protected:
		///
		/// Z-Level
		///
		unsigned int m_zLevel;

		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_vertexArray;

		///
		/// Vertex buffer.
		///
		qs::VertexBuffer m_vertexBuffer;

		///
		/// Index (Element) buffer.
		///
		qs::IndexBuffer m_indexBuffer;

		///
		/// Vertex layout.
		///
		qs::VertexLayout m_layout;
	};
}

#endif