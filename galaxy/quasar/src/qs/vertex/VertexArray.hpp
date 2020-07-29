///
/// VertexArrary.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXARRAY_HPP_
#define QUASAR_VERTEXARRAY_HPP_

#include "qs/vertex/IndexBuffer.hpp"
#include "qs/vertex/VertexBuffer.hpp"
#include "qs/vertex/VertexLayout.hpp"

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
		template<is_vertex VertexType>
		void create(qs::VertexBuffer& vb, qs::IndexBuffer& ib, const qs::VertexLayout& layout);

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

	template<is_vertex VertexType>
	inline void VertexArray::create(qs::VertexBuffer& vb, qs::IndexBuffer& ib, const qs::VertexLayout& layout)
	{
		bind();
		vb.bind();
		ib.bind();

		// Add each attribute in the layout to the vertex array object.
		// I.e. position attribute, then colour attribute of the verticies.
		const auto& attributes = layout.get_attributes();
		unsigned int counter   = 0;
		for (const auto& attribute : attributes)
		{
			glVertexAttribPointer(counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(VertexType), (GLvoid*)attribute.m_offset);
			glEnableVertexAttribArray(counter);

			++counter;
		}

		unbind();
		vb.unbind();
		ib.unbind();
	}
} // namespace qs

#endif