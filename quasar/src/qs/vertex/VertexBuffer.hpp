///
/// VertexBuffer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXBUFFER_HPP_
#define QUASAR_VERTEXBUFFER_HPP_

#include <array>
#include <vector>

#include <glad/glad.h>

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
		/// \brief Create vertex buffer object.
		///
		/// You will need to call bind() before using this buffer.
		///
		/// \param data Verticies to use.
		/// \param size Size of the verticies data array.
		/// \param glDrawType Type of gl drawing. i.e. GL_STATIC_DRAW.
		///
		template<std::size_t size>
		void create(const std::array<float, size>& data, unsigned int glDrawType = GL_STATIC_DRAW) noexcept;

		///
		/// \brief Create vertex buffer object.
		///
		/// You will need to call bind() before using this buffer.
		///
		/// \param data Verticies to use.
		/// \param glDrawType Type of gl drawing. i.e. GL_STATIC_DRAW.
		///
		void createFromDynamic(const std::vector<float>& data, unsigned int glDrawType = GL_STATIC_DRAW) noexcept;

		///
		/// Destroys buffer.
		///
		~VertexBuffer() noexcept;

		///
		/// Bind the current vertex buffer to current GL context.
		///
		void bind() const noexcept;

		///
		/// Unbind the current vertex buffer to current GL context.
		///
		void unbind() const noexcept;

	private:
		///
		/// ID returned by OpenGL when generating buffer.
		///
		unsigned int m_id;
	};

	template<std::size_t size>
	inline void VertexBuffer::create(const std::array<float, size>& data, unsigned int glDrawType) noexcept
	{
		// Gen a single buffer for this object.
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		// Copy data into buffer object.
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), glDrawType);

		// Clean up.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

#endif