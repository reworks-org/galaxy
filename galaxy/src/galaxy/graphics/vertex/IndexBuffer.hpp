///
/// IndexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_INDEXBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEX_INDEXBUFFER_HPP_

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
			///
			void create(std::span<unsigned int> indices);

			///
			/// Bind the current vertex buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current vertex buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Returns a const unsigned int.
			///
			[[nodiscard]] const unsigned int count() const noexcept;

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
			unsigned int m_id;

			///
			/// Count of indicies in array.
			///
			unsigned int m_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif