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
			UniformBuffer();

			///
			/// \brief Argument Constructor.
			///
			/// Calls create().
			///
			/// \param index Index of UBO in shader.
			///
			UniformBuffer(const unsigned int index);

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
			/// \param index Index of UBO in shader.
			///
			void create(const unsigned int index);

			///
			/// Reserve the buffer space.
			///
			/// \param size Size in bytes to reserve.
			///
			void reserve(const unsigned int size);

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
			void sub_buffer(const unsigned int offset, const unsigned int count, Type* data);

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

		template<meta::is_object Type>
		inline void UniformBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Type* data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, count * sizeof(Type), data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	} // namespace graphics
} // namespace galaxy

#endif