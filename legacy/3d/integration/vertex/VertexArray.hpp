///
/// VertexArrary.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEXARRAY_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEXARRAY_HPP_

#include "galaxy/graphics/vertex/Layout.hpp"
#include "galaxy/graphics/vertex/IndexBuffer.hpp"
#include "galaxy/graphics/vertex/VertexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL vertex array objects.
		///
		class VertexArray final
		{
		public:
			///
			/// Constructor.
			///
			VertexArray() noexcept;

			///
			/// Move constructor.
			///
			VertexArray(VertexArray&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexArray& operator=(VertexArray&&) noexcept;

			///
			/// Destructor.
			///
			~VertexArray() noexcept;

			///
			/// Create vertex array.
			///
			/// \param vb VertexBufferObject to specify for VertexArrayObject.
			/// \param ib IndexBufferObject to specify for VertexArrayObject.
			/// \param layout VertexLayout to specify for this VertexArrayObject.
			///
			template<meta::is_vertex VertexType>
			void create(VertexBuffer& vb, IndexBuffer& ib, const VertexLayout& layout);

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
			/// Copy constructor.
			///
			VertexArray(const VertexArray&) = delete;

			///
			/// Copy assignment operator.
			///
			VertexArray& operator=(const VertexArray&) = delete;

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

		template<meta::is_vertex VertexType>
		inline void VertexArray::create(VertexBuffer& vb, IndexBuffer& ib, const VertexLayout& layout)
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
	} // namespace graphics
} // namespace galaxy

#endif