///
/// VertexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEXBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEXBUFFER_HPP_

#include <vector>

#include "galaxy/graphics/vertex/Layout.hpp"

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
			/// Create vertex buffer object.
			///
			/// \param vertices Vertices to use.
			///
			template<meta::is_vertex VertexType>
			void create(std::vector<VertexType>& vertices);

			///
			/// Destroys buffer.
			///
			~VertexBuffer() noexcept;

			///
			/// Bind the current vertex buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current vertex buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// Get vertex storage.
			///
			/// \return Vertex storage.
			///
			template<meta::is_vertex VertexType>
			[[nodiscard]] std::vector<VertexType> get();

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
			unsigned int m_id;

			///
			/// Size of vertex buffer.
			///
			unsigned int m_size;
		};

		template<meta::is_vertex VertexType>
		inline void VertexBuffer::create(std::vector<VertexType>& vertices)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);

			if (!vertices.empty())
			{
				m_size = static_cast<unsigned int>(vertices.size());
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), vertices.data(), GL_DYNAMIC_DRAW);
			}
			else
			{
				m_size = static_cast<unsigned int>(vertices.capacity());
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), nullptr, GL_DYNAMIC_DRAW);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		template<meta::is_vertex VertexType>
		inline std::vector<VertexType> VertexBuffer::get()
		{
			std::vector<VertexType> vs;
			vs.reserve(m_size);

			glGetNamedBufferSubData(m_id, 0, m_size * sizeof(VertexType), &vs[0]);

			return vs;
		}
	} // namespace graphics
} // namespace galaxy

#endif