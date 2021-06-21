///
/// UniformBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_UNIFORMBUFFER_HPP_
#define GALAXY_GRAPHICS_UNIFORMBUFFER_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL vertex buffer objects.
		///
		class UniformBuffer final
		{
		public:
			///
			/// Constructor.
			///
			UniformBuffer() noexcept;

			///
			/// Move constructor.
			///
			UniformBuffer(UniformBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			UniformBuffer& operator=(UniformBuffer&&) noexcept;

			///
			/// Destroys buffer.
			///
			~UniformBuffer() noexcept;

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
			/// \param offset Offset to start adding buffer from.
			/// \param count Number of objects in data.
			/// \param data Pointer to the data to buffer.
			///
			template<typename Type>
			void sub_buffer(const unsigned int offset, const unsigned int count, Type* data);

			///
			/// Bind the uniform buffer object.
			///
			void bind() noexcept;

			///
			/// Unbind the uniform buffer object.
			///
			void unbind() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

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

		template<typename Type>
		inline void UniformBuffer::sub_buffer(const unsigned int offset, const unsigned int count, Type* data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, count * sizeof(Type), data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	} // namespace graphics
} // namespace galaxy

#endif