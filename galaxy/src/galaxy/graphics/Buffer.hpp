///
/// Buffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_BUFFER_HPP_
#define GALAXY_GRAPHICS_BUFFER_HPP_

#include <glad/glad.h>

#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL buffer objects.
		///
		/// \tparam gl_buffer_type Either GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.
		///
		template<unsigned int gl_buffer_type>
		class Buffer final
		{
			static_assert((gl_buffer_type == GL_UNIFORM_BUFFER) || (gl_buffer_type == GL_SHADER_STORAGE_BUFFER));

		  public:
			///
			/// Constructor.
			///
			Buffer();

			///
			/// Move constructor.
			///
			Buffer(Buffer&&);

			///
			/// Move assignment operator.
			///
			Buffer& operator=(Buffer&&);

			///
			/// Destructor.
			///
			~Buffer();

			///
			/// \brief Create the buffer.
			///
			/// Calls resize() for you.
			///
			/// \tparam Object The data structure to use with this buffer.
			///
			/// \param index Index of UBO in shader.
			///
			template<meta::is_class Object>
			void create(const unsigned int index);

			///
			/// Resizes the internal buffer.
			///
			/// \tparam Object The data structure to use with this buffer.
			///
			template<meta::is_class Object>
			void resize();

			///
			/// Resizes the internal buffer.
			///
			/// \param size Size of the data buffer.
			///
			void resize(const unsigned int size);

			///
			/// Sub buffer data.
			///
			/// \tparam Object The data structure to use with this buffer.
			///
			/// \param offset Offset to start adding buffer from.
			/// \param count Number of objects in data.
			/// \param data Pointer to the data to buffer.
			///
			template<meta::is_class Object>
			void buffer(const unsigned int offset, const unsigned int count, Object* data);

			///
			/// Bind buffer.
			///
			void bind();

			///
			/// Unbind buffer.
			///
			void unbind();

			///
			/// \brief Clears data from buffer.
			///
			/// You need to call resize() afterwards.
			///
			void reset();

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const;

		  private:
			///
			/// Copy constructor.
			///
			Buffer(const Buffer&) = delete;

			///
			/// Copy assignment operator.
			///
			Buffer& operator=(const Buffer&) = delete;

		  private:
			///
			/// OpenGL buffer id.
			///
			unsigned int m_obj;
		};

		template<unsigned int gl_buffer_type>
		inline Buffer<gl_buffer_type>::Buffer()
			: m_obj {0}
		{
			glGenBuffers(1, &m_obj);
		}

		template<unsigned int gl_buffer_type>
		inline Buffer<gl_buffer_type>::Buffer(Buffer&& b)
			: m_obj {0}
		{
			if (this->m_obj != 0)
			{
				glDeleteBuffers(1, &this->m_obj);
				this->m_obj = 0;
			}

			this->m_obj = b.m_obj;
			b.m_obj     = 0;
		}

		template<unsigned int gl_buffer_type>
		inline Buffer<gl_buffer_type>& Buffer<gl_buffer_type>::operator=(Buffer&& b)
		{
			if (this != &b)
			{
				if (this->m_obj != 0)
				{
					glDeleteBuffers(1, &this->m_obj);
					this->m_obj = 0;
				}

				this->m_obj = b.m_obj;
				b.m_obj     = 0;
			}

			return *this;
		}

		template<unsigned int gl_buffer_type>
		inline Buffer<gl_buffer_type>::~Buffer()
		{
			if (m_obj != 0)
			{
				glDeleteBuffers(1, &m_obj);
				m_obj = 0;
			}
		}

		template<unsigned int gl_buffer_type>
		template<meta::is_class Object>
		inline void Buffer<gl_buffer_type>::create(const unsigned int index)
		{
			glBindBuffer(gl_buffer_type, m_obj);
			glBindBufferBase(gl_buffer_type, index, m_obj);
			glBindBuffer(gl_buffer_type, 0);

			resize<Object>();
		}

		template<unsigned int gl_buffer_type>
		template<meta::is_class Object>
		inline void Buffer<gl_buffer_type>::resize()
		{
			constexpr auto size = sizeof(Object);

			GLint64 buffer_size = 0;
			glGetNamedBufferParameteri64v(m_obj, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_obj);
				glNamedBufferData(m_obj, size, nullptr, GL_DYNAMIC_DRAW);
			}
		}

		template<unsigned int gl_buffer_type>
		template<meta::is_class Object>
		inline void Buffer<gl_buffer_type>::buffer(const unsigned int offset, const unsigned int count, Object* data)
		{
			glNamedBufferSubData(m_obj, offset, count * sizeof(Object), data);
		}

		template<unsigned int gl_buffer_type>
		inline void Buffer<gl_buffer_type>::resize(const unsigned int size)
		{
			GLint64 buffer_size = 0;
			glGetNamedBufferParameteri64v(m_obj, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_obj);
				glNamedBufferData(m_obj, size, nullptr, GL_DYNAMIC_DRAW);
			}
		}

		template<unsigned int gl_buffer_type>
		inline void Buffer<gl_buffer_type>::bind()
		{
			glBindBuffer(gl_buffer_type, m_obj);
		}

		template<unsigned int gl_buffer_type>
		inline void Buffer<gl_buffer_type>::unbind()
		{
			glBindBuffer(gl_buffer_type, 0);
		}

		template<unsigned int gl_buffer_type>
		inline void Buffer<gl_buffer_type>::reset()
		{
			glInvalidateBufferData(m_obj);
		}

		template<unsigned int gl_buffer_type>
		inline unsigned int Buffer<gl_buffer_type>::id() const
		{
			return m_obj;
		}

		///
		/// OpenGL Uniform Buffer Object.
		///
		using UniformBuffer = Buffer<GL_UNIFORM_BUFFER>;

		///
		/// OpenGL Shader Storage Buffer Object.
		///
		using ShaderStorageBuffer = Buffer<GL_SHADER_STORAGE_BUFFER>;
	} // namespace graphics
} // namespace galaxy

#endif
