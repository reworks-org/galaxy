///
/// VertexArrary.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXARRAY_HPP_
#define GALAXY_GRAPHICS_VERTEXARRAY_HPP_

#include "galaxy/graphics/IndexBuffer.hpp"
#include "galaxy/graphics/InstanceBuffer.hpp"
#include "galaxy/graphics/VertexBuffer.hpp"

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
			/// \param vbo VBO to specify for VAO. Is move'd into this structure, the original is discarded.
			/// \param ibo IBO to specify for VAO. Is move'd into this structure, the original is discarded.
			///
			void create(VertexBuffer& vb, IndexBuffer& ib);

			///
			/// Bind the current vertex array to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current vertex array to current GL context.
			///
			void unbind() noexcept;

			///
			/// Enable instancing for this vertex array.
			///
			/// \param instance_buffer Instance buffer to use when instancing with this VAO.
			///						   This is not moved into the VAO, you need to keep the instance buffer alive seperately.
			///
			void set_instanced(InstanceBuffer& ibo) noexcept;

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Returns a const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

			///
			/// Get VAO GL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

			///
			/// Get VBO.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int vbo() const noexcept;

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
			unsigned int m_vao;

			///
			/// Keeps track of next free attribute id.
			///
			unsigned int m_counter;

			///
			/// Index Buffer.
			///
			IndexBuffer m_ibo;

			///
			/// Vertex Buffer.
			///
			VertexBuffer m_vbo;

			///
			/// Vertex layout.
			///
			VertexLayout m_layout;
		};
	} // namespace graphics
} // namespace galaxy

#endif