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
	template<typename VertexType>
	using VertexStorage = std::vector<VertexType>;

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
		///
		template<typename VertexType, typename BufferType>
		void create(const VertexStorage<VertexType>& vertices) noexcept;

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
		template<typename VertexType>
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
	
	template<typename VertexType, typename BufferType>
	inline void VertexBuffer::create(const VertexStorage<VertexType>& vertices) noexcept
	{
		// If not one of the two buffer type structs, throw compile-time assert.
		static_assert(std::is_same<BufferType, qs::BufferTypeDynamic>::value || std::is_same<BufferType, qs::BufferTypeStatic>::value);
		static_assert(std::is_same<VertexType, qs::SpriteVertex>::value || std::is_same<VertexType, qs::PrimitiveVertex>::value);

		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<BufferType, qs::BufferTypeDynamic>::value)
		{
			m_size = vertices.size();
			glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), nullptr, GL_DYNAMIC_DRAW);
		}
		else if constexpr (std::is_same<BufferType, qs::BufferTypeStatic>::value)
		{
			m_size = vertices.size();
			glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template<typename VertexType>
	inline decltype(auto) VertexBuffer::get() noexcept
	{
		VertexStorage<VertexType> vs;
		vs.reserve(m_size);

		glGetNamedBufferSubData(m_id, 0, m_size * sizeof(VertexType), &vs[0]);

		return std::move(vs);
	}
}

#endif