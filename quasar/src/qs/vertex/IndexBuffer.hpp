///
/// IndexBuffer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INDEXBUFFER_HPP_
#define QUASAR_INDEXBUFFER_HPP_

#include <array>
#include <vector>

#include "qs/utils/BufferType.hpp"

#include <glad/glad.h>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Custom index storage type.
	///
	template<std::size_t count>
	using IndexStorage = std::array<unsigned int, count>;

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
		/// Create index buffer object.
		///
		/// You will need to call bind() before using this buffer, once it is created.
		///
		/// \param indexs Index array to use.
		/// \param bufferType Fixed or dynamic buffer.
		///
		template<std::size_t count>
		void create(const qs::IndexStorage<count>& indexs, const qs::BufferType bufferType) noexcept;

		///
		/// Create index buffer from dynamic container.
		///
		/// \param indexs Index array to use.
		/// \param bufferType Fixed or dynamic buffer.
		///
		void create(const std::vector<unsigned int>& indexs, const qs::BufferType bufferType) noexcept;

		///
		/// Destroys buffer.
		///
		~IndexBuffer() noexcept;

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
		unsigned int getCount() const;

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

	template<std::size_t count>
	inline void IndexBuffer::create(const qs::IndexStorage<count>& indexs, const qs::BufferType bufferType) noexcept
	{
		m_count = count;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

		switch (bufferType)
		{
		case qs::BufferType::STATIC:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count, indexs.data(), GL_STATIC_DRAW);
			break;

		case qs::BufferType::DYNAMIC:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count, indexs.data(), GL_DYNAMIC_DRAW);
			break;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

#endif