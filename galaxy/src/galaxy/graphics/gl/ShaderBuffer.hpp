///
/// ShaderBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_SHADERBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_SHADERBUFFER_HPP_

#include <glad/glad.h>

#include "galaxy/graphics/Enums.hpp"

namespace galaxy
{
	///
	/// OpenGL Shader storage buffer.
	///
	class ShaderBuffer final
	{
	public:
		///
		/// \brief Constructor.
		///
		/// You must call set_index() if you use default constructor.
		///
		ShaderBuffer();

		///
		/// Constructor.
		///
		/// \param index Index binding that needs to match in vertex shader.
		///
		ShaderBuffer(const GLBufferBinding index);

		///
		/// Move constructor.
		///
		ShaderBuffer(ShaderBuffer&&) noexcept;

		///
		/// Move assignment operator.
		///
		ShaderBuffer& operator=(ShaderBuffer&&) noexcept;

		///
		/// Destructor.
		///
		~ShaderBuffer();

		///
		/// Set vertex shader binding point.
		///
		/// \param index Index binding that needs to match in vertex shader.
		///
		void set_index(const GLBufferBinding index);

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
		/// Copy constructor.
		///
		ShaderBuffer(const ShaderBuffer&) = delete;

		///
		/// Copy assignment operator.
		///
		ShaderBuffer& operator=(const ShaderBuffer&) = delete;

	private:
		///
		/// OpenGL handle.
		///
		unsigned int m_id;

		///
		/// Index binding of SSBO in vertex shader.
		///
		int m_index;
	};

	template<typename Object>
	inline void ShaderBuffer::buffer(const unsigned int count, Object* data)
	{
		clear();

		constexpr const auto size = sizeof(Object);
		glNamedBufferData(m_id, count * size, static_cast<void*>(data), GL_DYNAMIC_DRAW);
	}

	template<typename Object>
	inline void ShaderBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Object* data)
	{
		constexpr const auto size = sizeof(Object);
		glNamedBufferSubData(m_id, offset, count * size, static_cast<void*>(data));
	}
} // namespace galaxy

#endif
