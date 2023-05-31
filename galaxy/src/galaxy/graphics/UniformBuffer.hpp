///
/// UniformBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_UNIFORMBUFFER_HPP_
#define GALAXY_GRAPHICS_UNIFORMBUFFER_HPP_

#include <glad/glad.h>

#include "galaxy/meta/Concepts.hpp"
#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL uniform buffer objects.
		///
		class UniformBuffer final
		{
		public:
			///
			/// Constructor.
			///
			UniformBuffer();

			///
			/// Move constructor.
			///
			UniformBuffer(UniformBuffer&&);

			///
			/// Move assignment operator.
			///
			UniformBuffer& operator=(UniformBuffer&&);

			///
			/// Destructor.
			///
			~UniformBuffer();

			///
			/// Create the buffer.
			///
			/// \tparam Object The data structure to use with this buffer.
			///
			/// \param index Index of UBO in shader.
			///
			template<meta::is_class Object>
			void create(const unsigned int index);

			///
			/// Sub buffer data into UBO.
			///
			/// \tparam Type Is a class, union, array, scalar or integral constant.
			///
			/// \param offset Offset to start adding buffer from.
			/// \param count Number of objects in data.
			/// \param data Pointer to the data to buffer.
			///
			template<meta::is_object Type>
			void buffer(const unsigned int offset, const unsigned int count, Type* data);

			///
			/// Bind UBO.
			///
			void bind();

			///
			/// Unbind UBO.
			///
			void unbind();

			///
			/// \brief Reset all UBO data.
			///
			/// You will need to call create() and reserve() again.
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
			UniformBuffer(const UniformBuffer&) = delete;
			///
			/// Copy assignment operator.
			///
			UniformBuffer& operator=(const UniformBuffer&) = delete;

		private:
			///
			/// ID returned by OpenGL when generating buffer.
			///
			unsigned int m_ubo;
		};

		template<meta::is_class Object>
		inline void UniformBuffer::create(const unsigned int index)
		{
			constexpr auto size = sizeof(Object);

			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ubo);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_ubo);
				glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		template<meta::is_object Type>
		inline void UniformBuffer::buffer(const unsigned int offset, const unsigned int count, Type* data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, count * sizeof(Type), data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	} // namespace graphics
} // namespace galaxy

#endif