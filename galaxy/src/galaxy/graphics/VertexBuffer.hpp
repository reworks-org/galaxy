///
/// VertexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEXBUFFER_HPP_

#include <span>

#include "galaxy/graphics/VertexLayout.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL vertex buffer objects.
		///
		class VertexBuffer final
		{
		public:
			///
			/// Constructor.
			///
			VertexBuffer() noexcept;

			///
			/// Move constructor.
			///
			VertexBuffer(VertexBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexBuffer& operator=(VertexBuffer&&) noexcept;

			///
			/// Destroys buffer.
			///
			~VertexBuffer() noexcept;

			///
			/// Create vertex buffer object.
			///
			/// \param vertices Vertices to assign.
			/// \param single_write Flag to mark that this buffer wont be updated after creation.
			/// \param capacity Use this to allocate the buffer if you dont want to supply the vertices at this point. Is ignored if vertices are not empty.
			///
			void create(std::span<Vertex> vertices, bool single_write = false, unsigned int capacity = 1);

			///
			/// Sub-buffer vertex object.
			///
			/// \param vertices Vertices to assign.
			/// \param index Offset to start at from initial vertices. 0 = first element.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

			///
			/// Bind the current vertex buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current vertex buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// \brief Destroy Vertex Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			VertexBuffer(const VertexBuffer&) = delete;
			///
			/// Copy assignment operator.
			///
			VertexBuffer& operator=(const VertexBuffer&) = delete;

		private:
			///
			/// ID returned by OpenGL when generating buffer.
			///
			unsigned int m_vbo;

			///
			/// Size of vertex buffer.
			///
			unsigned int m_size;

			///
			/// CPU-side cache of vertices.
			///
			std::vector<Vertex> m_vertices;
		};
	} // namespace graphics
} // namespace galaxy

#endif