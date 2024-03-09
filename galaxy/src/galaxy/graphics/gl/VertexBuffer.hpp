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
			VertexBuffer();

			///
			/// Move constructor.
			///
			VertexBuffer(VertexBuffer&&);

			///
			/// Move assignment operator.
			///
			VertexBuffer& operator=(VertexBuffer&&);

			///
			/// Destructor.
			///
			~VertexBuffer();

			///
			/// Create vertex buffer object.
			///
			/// \param vertices Vertices to assign.
			/// \param indicies Indices for vertex buffer.
			///
			void buffer(std::span<Vertex> vertices, std::span<unsigned int> indicies);

			///
			/// Create vertex data without uploading.
			///
			/// \param vertex_count Size of vertices.
			/// \param indicies Indices for vertex buffer.
			///
			void buffer(const int vertex_count, std::span<unsigned int> indicies);

			///
			/// Sub-buffer vertex object.
			///
			/// \param index Offset to start at from initial vertices. 0 = first element.
			/// \param vertices Vertices to assign.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

			///
			/// Clear buffer data.
			///
			void clear();

			///
			/// Get the index count.
			///
			/// \return Integer.
			///
			[[nodiscard]] int count() const;

			///
			/// Gets index offset.
			///
			/// \return Integer as void pointer for opengl shenanigans.
			///
			[[nodiscard]] void* offset();

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const;

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
	} // namespace graphics
} // namespace galaxy

#endif
