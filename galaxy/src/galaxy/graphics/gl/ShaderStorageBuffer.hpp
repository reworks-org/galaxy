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
	namespace graphics
	{
		///
		/// Abstraction for OpenGL buffer objects.
		///
		class ShaderStorageBuffer final
		{
		  public:
			///
			/// Constructor.
			///
			/// \param index Index to set SSBO to.
			///
			ShaderStorageBuffer(const int index);

			///
			/// Move constructor.
			///
			ShaderStorageBuffer(ShaderStorageBuffer&&);

			///
			/// Move assignment operator.
			///
			ShaderStorageBuffer& operator=(ShaderStorageBuffer&&);

			///
			/// Destructor.
			///
			~ShaderStorageBuffer();

			///
			/// Buffer data.
			///
			/// \tparam Object The data structure to use with this buffer.
			///
			/// \param offset Offset to start adding buffer from.
			/// \param count Number of objects in data.
			/// \param data Pointer to the data to buffer.
			///
			template<typename Object>
			void buffer(const unsigned int offset, const unsigned int count, Object* data);

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
			/// Clears data from buffer.
			///
			void clear();

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const;

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
			/// Index of SSBO in shader.
			///
			int m_index;
		};

		template<typename Object>
		inline void ShaderStorageBuffer::buffer(const unsigned int offset, const unsigned int count, Object* data)
		{
			constexpr auto size = sizeof(Object);
			glNamedBufferData(m_id, count * size, data, GL_DYNAMIC_DRAW);
		}

		template<typename Object>
		inline void ShaderStorageBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Object* data)
		{
			constexpr auto size = sizeof(Object);
			glNamedBufferSubData(m_id, offset, count * size, data);
		}
	} // namespace graphics
} // namespace galaxy

#endif
