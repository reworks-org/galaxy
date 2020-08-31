///
/// IndexBuffer.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INDEXBUFFER_HPP_
#define QUASAR_INDEXBUFFER_HPP_

#include <array>
#include <span>
#include <vector>

#include <glad/glad.h>

#include "qs/utils/Meta.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Abstraction for OpenGL index (element) buffer objects.
	///
	class IndexBuffer final
	{
	public:
		///
		/// Default constructor.
		///
		IndexBuffer() noexcept;

		///
		/// Copy constructor.
		///
		IndexBuffer(const IndexBuffer&) noexcept = delete;

		///
		/// Move constructor.
		///
		IndexBuffer(IndexBuffer&&);

		///
		/// Copy assignment operator.
		///
		IndexBuffer& operator=(const IndexBuffer&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		IndexBuffer& operator=(IndexBuffer&&);

		///
		/// Destroys buffer.
		///
		~IndexBuffer() noexcept;

		///
		/// Create index buffer object.
		///
		/// \param indexs Index array to use.
		///
		template<is_buffer BufferType>
		void create(std::span<unsigned int> indexs);

		///
		/// Bind the current vertex buffer to current GL context.
		///
		void bind() noexcept;

		///
		/// Unbind the current vertex buffer to current GL context.
		///
		void unbind() noexcept;

		///
		/// Get the count of indicies in the index buffer.
		///
		/// \return Returns a const unsigned int.
		///
		unsigned int count() const noexcept;

	private:
		///
		/// ID returned by OpenGL when generating buffer.
		///
		unsigned int m_id;

		///
		/// Count of indicies in array.
		///
		unsigned int m_count;
	};

	template<is_buffer BufferType>
	inline void IndexBuffer::create(std::span<unsigned int> indexs)
	{
		m_count = static_cast<unsigned int>(indexs.size());
		bind();

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<BufferType, qs::BufferDynamic>::value)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_DYNAMIC_DRAW);
		}
		else
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_STATIC_DRAW);
		}

		unbind();
	}
} // namespace qs

#endif