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
			UniformBuffer() noexcept;

			///
			/// \brief Argument Constructor.
			///
			/// Calls create().
			///
			/// \param index Index of UBO in shader.
			///
			UniformBuffer(const unsigned int index) noexcept;

			///
			/// Move constructor.
			///
			UniformBuffer(UniformBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			UniformBuffer& operator=(UniformBuffer&&) noexcept;

			///
			/// Destructor.
			///
			~UniformBuffer() noexcept;

			///
			/// Create the buffer.
			///
			/// \param index Index of UBO in shader.
			///
			void create(const unsigned int index) noexcept;

			///
			/// Reserve the buffer space.
			///
			/// \param size Size in bytes to reserve.
			///
			void reserve(const unsigned int size) noexcept;

			///
			/// Sub buffer data into UBO.
			///
			/// \param offset Offset to start adding buffer from.
			/// \param count Number of objects in data.
			/// \param data Pointer to the data to buffer.
			///
			template<meta::is_arithmetic Type>
			void sub_buffer(const unsigned int offset, const unsigned int count, Type* data) noexcept;

			///
			/// Bind UBO.
			///
			void bind() noexcept;

			///
			/// Unbind UBO.
			///
			void unbind() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const noexcept;

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

		template<meta::is_arithmetic Type>
		inline void UniformBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Type* data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, count * sizeof(Type), data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	} // namespace graphics
} // namespace galaxy

#endif