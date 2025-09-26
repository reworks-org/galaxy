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
		/// Create vertex array object.
		///
		/// \param vertices Vertices to assign.
		/// \param indicies Indices for vertex buffer.
		///
		void buffer(std::span<Vertex> vertices, std::span<unsigned int> indicies);

		///
		/// Create vertex array without uploading data.
		///
		/// \param vertex_count Size of vertices.
		/// \param indicies Indices for vertex buffer.
		///
		void buffer(const int vertex_count, std::span<unsigned int> indicies);

		///
		/// Sub-buffer vertex array.
		///
		/// \param index Offset to start at from initial vertices. 0 = first element.
		/// \param vertices Vertices to assign.
		///
		void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

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
