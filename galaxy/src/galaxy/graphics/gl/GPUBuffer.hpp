///
/// GPUBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_GPUBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_GPUBUFFER_HPP_

#include <glad/glad.h>

#include "galaxy/graphics/Enums.hpp"

namespace galaxy
{
	///
	/// OpenGL GPU-Side buffer, such as MDI, SSBO, UBO, EBO, etc.
	///
	class GPUBuffer final
	{
	public:
		///
		/// Constructor.
		///
		/// \param index Index binding that needs to match in vertex shader.
		/// \param type Buffer type, such as GL_SHADER_STORAGE_BUFFER.
		///
		GPUBuffer(const GLBufferBinding index, const int type);

		///
		/// Move constructor.
		///
		GPUBuffer(GPUBuffer&&) noexcept;

		///
		/// Move assignment operator.
		///
		GPUBuffer& operator=(GPUBuffer&&) noexcept;

		///
		/// Destructor.
		///
		~GPUBuffer();

		///
		/// Buffer data.
		///
		/// \tparam Object The data structure to use with this buffer.
		///
		/// \param count Number of objects in data.
		/// \param data Pointer to the data to buffer.
		///
		template<typename Object>
		void buffer(const unsigned int count, Object* data);

		///
		/// Sub buffer data.
		///
		/// \tparam Object The data structure to use with this buffer.
		///
		/// \param offset Offset to start adding buffer from.
		/// \param count Number of objects in data.
		/// \param data Pointer to the data to buffer.
		///
		template<typename Object>
		void sub_buffer(const unsigned int offset, const unsigned int count, Object* data);

		///
		/// Bind buffer.
		///
		void bind() const;

		///
		/// Unbind buffer.
		///
		void unbind() const;

		///
		/// Clears data from buffer.
		///
		void clear() const;

		///
		/// Destroy shader.
		///
		void destroy();

		///
		/// Get OpenGL handle.
		///
		/// \return Unsigned integer.
		///
		[[nodiscard]]
		unsigned int id() const noexcept;

	private:
		///
		/// Default constructor.
		///
		GPUBuffer() = delete;

		///
		/// Copy constructor.
		///
		GPUBuffer(const GPUBuffer&) = delete;

		///
		/// Copy assignment operator.
		///
		GPUBuffer& operator=(const GPUBuffer&) = delete;

	private:
		///
		/// OpenGL handle.
		///
		unsigned int m_id;

		///
		/// Index binding of SSBO in vertex shader.
		///
		int m_index;

		///
		/// Buffer type.
		///
		int m_type;
	};

	template<typename Object>
	inline void GPUBuffer::buffer(const unsigned int count, Object* data)
	{
		clear();

		constexpr auto size = sizeof(Object);
		glNamedBufferData(m_id, count * size, (void*)data, GL_DYNAMIC_DRAW);
	}

	template<typename Object>
	inline void GPUBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Object* data)
	{
		constexpr auto size = sizeof(Object);
		glNamedBufferSubData(m_id, offset, count * size, (void*)data);
	}
} // namespace galaxy

#endif
