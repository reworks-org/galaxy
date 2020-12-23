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
		VertexArray();

		///
		/// Copy constructor.
		///
		VertexArray(const VertexArray&) = delete;

		///
		/// Move constructor.
		///
		VertexArray(VertexArray&&);

		///
		/// Copy assignment operator.
		///
		VertexArray& operator=(const VertexArray&) = delete;

		///
		/// Move assignment operator.
		///
		VertexArray& operator=(VertexArray&&);

		///
		/// Destroys buffer.
		///
		~VertexArray();

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
		/// \brief Set as an instanced vertex array.
		///
		/// You should call this after create().
		///
		/// \param ib InstanceBuffer object. You need to store this elsewhere.
		///
		void set_instanced(qs::InstanceBuffer& ib);

		///
		/// Bind the current vertex array to current GL context.
		///
		void bind();

		///
		/// Unbind the current vertex array to current GL context.
		///
		void unbind();

	private:
		///
		/// ID returned by OpenGL when generating vertex array.
		///
		unsigned int m_id;

		///
		/// Keeps track of next free attribute id.
		///
		unsigned int m_counter;
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
		for (const auto& attribute : attributes)
		{
			glVertexAttribPointer(m_counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(VertexType), (GLvoid*)attribute.m_offset);
			glEnableVertexAttribArray(m_counter);

			++m_counter;
		}

		unbind();
		vb.unbind();
		ib.unbind();
	}
} // namespace qs

#endif