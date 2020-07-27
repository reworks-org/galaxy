///
/// VertexBuffer.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXBUFFER_HPP_
#define QUASAR_VERTEXBUFFER_HPP_

#include "qs/vertex/VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Shorthand.
	///
	template<is_vertex vertex_type>
	using VertexStorage = std::vector<vertex_type>;

	///
	/// Shorthand.
	///
	template<is_vertex vertex_type>
	using VertexQuadStorage = std::array<vertex_type, 4>;

	///
	/// Abstraction for OpenGL vertex buffer objects.
	///
	class VertexBuffer final
	{
	public:
		///
		/// Default constructor.
		///
		VertexBuffer() noexcept;

		///
		/// Destroys buffer.
		///
		~VertexBuffer() noexcept;

		///
		/// Create vertex buffer object.
		///
		/// \param vertices Vertexs to use.
		/// \param dynamic_verticies Optional. True if the vertices should be copied into the OpenGL buffer for dynamic draw types.
		///
		template<is_vertex vertex_type, is_buffer buffer_type>
		void create(const VertexStorage<vertex_type>& vertices, bool dynamic_verticies = true);

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
		/// \return Vertex storage as a float array pointer.
		///
		template<typename vertex_type>
		decltype(auto) get() noexcept;

		///
		/// Get OpenGL handle.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int id() const noexcept;

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

	template<is_vertex vertex_type, is_buffer buffer_type>
	inline void VertexBuffer::create(const VertexStorage<vertex_type>& vertices, bool dynamic_verticies)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		if (!vertices.empty())
		{
			m_size = static_cast<unsigned int>(vertices.size());
		}
		else
		{
			m_size = static_cast<unsigned int>(vertices.capacity());
		}

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<buffer_type, qs::BufferDynamic>::value)
		{
			if (dynamic_verticies)
			{
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(vertex_type), vertices.data(), GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(vertex_type), nullptr, GL_DYNAMIC_DRAW);
			}
		}
		else if constexpr (std::is_same<buffer_type, qs::BufferStatic>::value)
		{
			glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(vertex_type), vertices.data(), GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template<typename vertex_type>
	inline decltype(auto) VertexBuffer::get() noexcept
	{
		VertexStorage<vertex_type> vs;
		vs.reserve(m_size);

		glGetNamedBufferSubData(m_id, 0, m_size * sizeof(vertex_type), &vs[0]);

		return vs;
	}
} // namespace qs

#endif