///
/// IndexBuffer.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INDEXBUFFER_HPP_
#define QUASAR_INDEXBUFFER_HPP_

#include <array>

#include "qs/libs/glad/glad.h"

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
		/// Create index buffer object.
		///
		/// You will need to call bind() before using this buffer, once it is created.
		///
		/// \param data Indice array to use.
		/// \param glDrawType Type of gl drawing. i.e. GL_STATIC_DRAW.
		///
		template<std::size_t size>
		void create(const std::array<unsigned int, size>& indices, unsigned int glDrawType) noexcept;

		///
		/// Destroys buffer.
		///
		~IndexBuffer() noexcept;

		///
		/// Bind the current vertex buffer to current GL context.
		///
		void bind() const noexcept;

		///
		/// Unbind the current vertex buffer to current GL context.
		///
		void unbind() const noexcept;

		///
		/// Get the count of indicies in the index buffer.
		///
		/// \return Returns a const unsigned int.
		///
		unsigned int count() const;

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

	template<std::size_t size>
	inline void IndexBuffer::create(const std::array<unsigned int, size>& indices, unsigned int glDrawType) noexcept
	{
		m_count = static_cast<unsigned int>(indices.size());

		// Gen a single buffer for this object.
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

		// Copy data into buffer object.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), glDrawType);

		// Clean up.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

#endif