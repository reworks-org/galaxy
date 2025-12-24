///
/// VertexArrary.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_VERTEXARRAY_HPP_
#define GALAXY_GRAPHICS_GL_VERTEXARRAY_HPP_

#include "galaxy/graphics/gl/VertexBuffer.hpp"

namespace galaxy
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
		VertexArray();

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
		~VertexArray();

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
		void sub_buffer(const unsigned int vi, const int vertex_size, const std::span<Vertex> vertices, unsigned int ei, const int index_size, std::span<unsigned int> indices);

		///
		/// Erase a specfic segment of data.
		///
		/// \param vi Offset to start at from initial vertices. 0 = first.
		/// \param vertex_count Number of vertices.
		/// \param ei Offset to start at from initial indices. 0 = first.
		/// \param index_count Number of indices.
		///
		void erase(const unsigned int vi, const int vertex_count, const unsigned int ei, const int index_count);

		///
		/// Clear buffer data.
		///
		void clear();

		///
		/// Bind this vertex array.
		///
		void bind() const;

		///
		/// Unbind this vertex array.
		///
		void unbind() const;

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
		/// Get vertex buffer.
		///
		/// \return Reference to vertex buffer object.
		///
		[[nodiscard]]
		VertexBuffer& vbo() noexcept;

		///
		/// Get vertex array handle.
		///
		/// \return Unsigned int.
		///
		[[nodiscard]]
		unsigned int id() const noexcept;

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
		/// VAO object.
		///
		unsigned int m_id;

		///
		/// Vertex buffer.
		///
		VertexBuffer m_vbo;
	};
} // namespace galaxy

#endif
