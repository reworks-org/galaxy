///
/// VertexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEXBUFFER_HPP_

#include <span>
#include <vector>

#include "galaxy/graphics/StorageFlags.hpp"
#include "galaxy/graphics/Vertex.hpp"

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
			/// Argument Constructor.
			///
			/// \brief Initialize buffer with capacity.
			///
			/// \param capacity Capacity to assign to this buffer.
			/// \param flag Static or dynamic data buffer.
			///
			VertexBuffer(const unsigned int capacity, const StorageFlag flag) noexcept;

			///
			/// Move constructor.
			///
			VertexBuffer(VertexBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexBuffer& operator=(VertexBuffer&&) noexcept;

			///
			/// Destructor.
			///
			~VertexBuffer() noexcept;

			///
			/// Create vertex buffer object.
			///
			/// \param vertices Vertices to assign.
			/// \param flag Static or dynamic data buffer.
			///
			void create(std::span<Vertex> vertices, const StorageFlag flag);

			///
			/// Sub-buffer vertex object.
			///
			/// \param index Offset to start at from initial vertices. 0 = first element.
			/// \param vertices Vertices to assign.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

			///
			/// Clear all data from VBO.
			///
			void clear() noexcept;

			///
			/// \brief Destroy Vertex Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const noexcept;

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
			/// CPU side cache of vertex data.
			///
			std::vector<Vertex> m_vertices;
		};
	} // namespace graphics
} // namespace galaxy

#endif