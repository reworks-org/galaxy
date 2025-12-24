///
/// VertexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_VERTEXBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_VERTEXBUFFER_HPP_

#include <span>

#include "galaxy/graphics/gl/Vertex.hpp"

namespace galaxy
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
		VertexBuffer();

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
		~VertexBuffer();

		///
		/// Create vertex buffer.
		///
		/// \param vertices Vertices to assign.
		/// \param indices Indices for vertex buffer.
		///
		void buffer(std::span<Vertex> vertices, std::span<unsigned int> indices);

		///
		/// Create vertex buffer without uploading.
		///
		/// \param vertex_count Number of vertices.
		/// \param index_count Number of indices.
		///
		void reserve(const int vertex_count, const int index_count);

		///
		/// Sub-buffer vertex buffer.
		///
		/// \param vi Offset to start at from initial vertices. 0 = first.
		/// \param vertex_size Amount of vertex data to sub buffer.
		/// \param vertices Vertices to assign.
		/// \param ei Offset to start at from initial indices. 0 = first.
		/// \param index_size Amount of index data to sub buffer.
		/// \param indices Indices to assign.
		///
		void sub_buffer(const unsigned int vi, const int vertex_size, const std::span<Vertex> vertices, unsigned int ei, const int index_size, std::span<unsigned int> indices) const;

		///
		/// Erase a specfic segment of data.
		///
		/// \param vi Offset to start at from initial vertices. 0 = first.
		/// \param vertex_count Number of vertices.
		/// \param ei Offset to start at from initial indices. 0 = first.
		/// \param index_count Number of indices.
		///
		void erase(const unsigned int vi, const int vertex_count, const unsigned int ei, const int index_count) const;

		///
		/// Clear buffer data.
		///
		void clear() const;

		///
		/// Get the index count.
		///
		/// \return Integer.
		///
		[[nodiscard]]
		int count() const noexcept;

		///
		/// Gets index offset.
		///
		/// \return Integer as void pointer for opengl shenanigans.
		///
		[[nodiscard]]
		void* offset() noexcept;

		///
		/// Get OpenGL handle.
		///
		/// \return Unsigned integer.
		///
		[[nodiscard]]
		unsigned int id() const noexcept;

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
		unsigned int m_id;

		///
		/// Index buffer offset.
		///
		std::size_t m_offset;

		///
		/// Index buffer count.
		///
		int m_count;
	};
} // namespace galaxy

#endif
