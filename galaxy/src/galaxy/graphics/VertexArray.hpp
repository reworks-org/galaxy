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
			/// \param vertices Vertices to assign.
			/// \param vertices_flag Static or dynamic data buffer.
			/// \param indices Indices to assign.
			/// \param indices_flag Static or dynamic data buffer.
			///
			void create(std::span<Vertex> vertices, const StorageFlag vertices_flag, std::span<unsigned int> indices, const StorageFlag indices_flag) noexcept;

			///
			/// Enable instancing for this vertex array.
			///
			/// \param offsets Offset position for each vertex.
			///
			void set_instanced(std::span<glm::vec2> offsets);

			///
			/// Sub-buffer vertex object.
			///
			/// \param index Offset to start at from initial vertices. 0 = first element.
			/// \param vertices Vertices to assign.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

			///
			/// \brief Destroy Vertex Array Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

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
			VertexBuffer m_vbo;

			///
			/// Index buffer.
			///
			IndexBuffer m_ibo;

			///
			/// Instance buffer.
			///
			InstanceBuffer m_instance_buffer;
		};
	} // namespace graphics
} // namespace galaxy

#endif