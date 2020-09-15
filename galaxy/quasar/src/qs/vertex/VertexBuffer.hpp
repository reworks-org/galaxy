///
/// VertexBuffer.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXBUFFER_HPP_
#define QUASAR_VERTEXBUFFER_HPP_

#include <span>

#include "qs/vertex/VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
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
		/// Copy constructor.
		///
		VertexBuffer(const VertexBuffer&) noexcept = delete;

		///
		/// Move constructor.
		///
		VertexBuffer(VertexBuffer&&);

		///
		/// Copy assignment operator.
		///
		VertexBuffer& operator=(const VertexBuffer&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		VertexBuffer& operator=(VertexBuffer&&);

		///
		/// Create vertex buffer object.
		///
		/// \param vertices Vertexs to use.
		/// \param dynamic_verticies Optional. True if the vertices should be copied into the OpenGL buffer for dynamic draw types.
		///
		template<is_vertex VertexType, is_buffer BufferType>
		void create(std::span<VertexType> vertices, bool dynamic_verticies = true);

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
		template<typename VertexType>
		[[nodiscard]] std::vector<VertexType> get() noexcept;

		///
		/// Get OpenGL handle.
		///
		/// \return Const unsigned integer.
		///
		[[nodiscard]] const unsigned int id() const noexcept;

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

	template<is_vertex VertexType, is_buffer BufferType>
	inline void VertexBuffer::create(std::span<VertexType> vertices, bool dynamic_verticies)
	{
		if (!vertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);

			m_size = static_cast<unsigned int>(vertices.size());

			// Now to use constexpr to check on compile time the buffer type.
			// This is faster since we dont need to bother checking at runtime.
			// constexpr will discard the branch that is false and it wont be compiled.
			if constexpr (std::is_same<BufferType, qs::BufferDynamic>::value)
			{
				if (dynamic_verticies)
				{
					glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), vertices.data(), GL_DYNAMIC_DRAW);
				}
				else
				{
					glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), nullptr, GL_DYNAMIC_DRAW);
				}
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else
		{
			PL_LOG(PL_WARNING, "Passed empty verticies to vertex buffer.");
		}
	}

	template<typename VertexType>
	inline std::vector<VertexType> VertexBuffer::get() noexcept
	{
		std::vector<VertexType> vs;
		vs.reserve(m_size);

		glGetNamedBufferSubData(m_id, 0, m_size * sizeof(VertexType), &vs[0]);

		return vs;
	}
} // namespace qs

#endif