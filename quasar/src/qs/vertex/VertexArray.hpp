///
/// VertexArrary.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXARRAY_HPP_
#define QUASAR_VERTEXARRAY_HPP_

#include "qs/vertex/IndexBuffer.hpp"
#include "qs/vertex/VertexBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec.
	///
	class VertexLayout;
	class Transform;

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
		/// Creation constructor (with indexbuffer).
		///
		/// \param vb VertexBufferObject to specify for VertexArrayObject.
		/// \param ib IndexBufferObject to specify for VertexArrayObject.
		/// \param layout VertexLayout to specify for this VertexArrayObject.
		///
		explicit VertexArray(qs::VertexBuffer& vb, qs::IndexBuffer& ib, qs::VertexLayout& layout) noexcept;

		///
		/// Destroys buffer.
		///
		~VertexArray() noexcept;

		///
		/// Create vertex array.
		///
		/// \param vb VertexBufferObject to specify for VertexArrayObject.
		/// \param ib IndexBufferObject to specify for VertexArrayObject.
		/// \param layout VertexLayout to specify for this VertexArrayObject.
		///
		void create(qs::VertexBuffer& vb, qs::IndexBuffer& ib, qs::VertexLayout& layout) noexcept;

		///
		/// Bind the current vertex array to current GL context.
		///
		void bind() noexcept;

		///
		/// Unbind the current vertex array to current GL context.
		///
		void unbind() noexcept;

	private:
		///
		/// ID returned by OpenGL when generating vertex array.
		///
		unsigned int m_id;
	};
}

#endif