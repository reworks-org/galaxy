///
/// VertexArrary.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXARRAY_HPP_
#define GALAXY_GRAPHICS_VERTEXARRAY_HPP_

#include <memory>

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
			/// \param vb VBO to specify for VAO. Takes ownership.
			/// \param ib IBO to specify for VAO. Takes ownership.
			///
			void create(std::unique_ptr<VertexBuffer>& vb, std::unique_ptr<IndexBuffer>& ib) noexcept;

			///
			/// Enable instancing for this vertex array.
			///
			/// \param instancing Instance buffer to use when instancing with this VAO. Takes ownership.
			///
			void set_instanced(std::unique_ptr<InstanceBuffer> instancing) noexcept;

			///
			/// Bind this vertex array to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind this vertex array to current GL context.
			///
			void unbind() noexcept;

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Integer.
			///
			[[nodiscard]] int index_count() const noexcept;

			///
			/// Get VAO GL id.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const noexcept;

			///
			/// Get VBO.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int vbo() const noexcept;

			///
			/// Get IBO.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int ibo() const noexcept;

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
			/// OpenGL id.
			///
			unsigned int m_vao;

			///
			/// Vertex buffer.
			///
			std::unique_ptr<VertexBuffer> m_vbo;

			///
			/// Index buffer.
			///
			std::unique_ptr<IndexBuffer> m_ibo;

			///
			/// Instance buffer.
			///
			std::unique_ptr<InstanceBuffer> m_instance_buffer;
		};
	} // namespace graphics
} // namespace galaxy

#endif