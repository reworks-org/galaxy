///
/// ShaderStorageBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_SHADERSTORAGEBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_SHADERSTORAGEBUFFER_HPP_

#include <glad/glad.h>

namespace galaxy
{
	///
	/// \brief Modern SSBO abstraction.
	///
	/// https://ktstephano.github.io/rendering/opengl/ssbos
	///
	class ShaderStorageBuffer final
	{
	public:
		///
		/// Constructor.
		///
		/// \param index Index binding that needs to match in vertex shader.
		///
		ShaderStorageBuffer(const int index);

		///
		/// Move constructor.
		///
		ShaderStorageBuffer(ShaderStorageBuffer&&) noexcept;

		///
		/// Move assignment operator.
		///
		ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) noexcept;

		///
		/// Destructor.
		///
		~ShaderStorageBuffer();

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
		ShaderStorageBuffer() = delete;

		///
		/// Copy constructor.
		///
		ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;

		///
		/// Copy assignment operator.
		///
		ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;

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
	inline void ShaderStorageBuffer::buffer(const unsigned int count, Object* data)
	{
		clear();

		constexpr auto size = sizeof(Object);
		glNamedBufferData(m_id, count * size, (void*)data, GL_DYNAMIC_DRAW);
	}

	template<typename Object>
	inline void ShaderStorageBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Object* data)
	{
		constexpr auto size = sizeof(Object);
		glNamedBufferSubData(m_id, offset, count * size, (void*)data);
	}
} // namespace galaxy

#endif
