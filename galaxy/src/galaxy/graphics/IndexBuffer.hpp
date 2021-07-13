///
/// IndexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_INDEXBUFFER_HPP_
#define GALAXY_GRAPHICS_INDEXBUFFER_HPP_

#include <span>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL index (element) buffer objects.
		///
		class IndexBuffer final
		{
		public:
			///
			/// Constructor.
			///
			IndexBuffer() noexcept;

			///
			/// Move constructor.
			///
			IndexBuffer(IndexBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			IndexBuffer& operator=(IndexBuffer&&) noexcept;

			///
			/// Destroys buffer.
			///
			~IndexBuffer() noexcept;

			///
			/// Create index buffer object.
			///
			/// \param indices Index array to use.
			/// \param single_write Flag to mark that this buffer wont be updated after creation.
			///
			void create(std::span<unsigned int> indices, const bool single_write);

			///
			/// Bind the current vertex buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current vertex buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// \brief Destroy Index Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Returns a const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			IndexBuffer(const IndexBuffer&) = delete;

			///
			/// Copy assignment operator.
			///
			IndexBuffer& operator=(const IndexBuffer&) = delete;

		private:
			///
			/// ID returned by OpenGL when generating buffer.
			///
			unsigned int m_ibo;

			///
			/// Count of indicies in array.
			///
			unsigned int m_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif