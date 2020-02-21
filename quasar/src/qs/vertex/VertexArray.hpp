///
/// VertexArrary.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXARRAY_HPP_
#define QUASAR_VERTEXARRAY_HPP_

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Abstraction for OpenGL vertex array objects.
	///
	class VertexArray final
	{
	public:
		///
		/// Default constructor.
		///
		VertexArray() noexcept;

		///
		/// Creation constructor.
		///
		/// \param vb VertexBufferObject to bind to VertexArrayObject.
		/// \param ib IndexBufferObject to bind to VertexArrayObject.
		/// \param layout VertexLayout to specify for this VertexArrayObject.
		///
		VertexArray(const qs::VertexBuffer& vb, const qs::IndexBuffer& ib, const qs::VertexLayout& layout) noexcept;

		///
		/// Create vertex array.
		///
		/// \param vb VertexBufferObject to bind to VertexArrayObject.
		/// \param ib IndexBufferObject to bind to VertexArrayObject.
		/// \param layout VertexLayout to specify for this VertexArrayObject.
		///
		void create(const qs::VertexBuffer& vb, const qs::IndexBuffer& ib, const qs::VertexLayout& layout) noexcept;

		///
		/// Destroys buffer.
		///
		~VertexArray() noexcept;

		///
		/// Bind the current vertex array to current GL context.
		///
		void bind() const noexcept;

		///
		/// Unbind the current vertex array to current GL context.
		///
		void unbind() const noexcept;

	private:
		///
		/// ID returned by OpenGL when generating vertex array.
		///
		unsigned int m_id;
	};
}

#endif