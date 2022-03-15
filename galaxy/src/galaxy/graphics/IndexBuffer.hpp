///
/// IndexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_INDEXBUFFER_HPP_
#define GALAXY_GRAPHICS_INDEXBUFFER_HPP_

#include <span>

#include "galaxy/graphics/StorageFlags.hpp"

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
			/// Argument Constructor.
			///
			/// \brief Initialize buffer with capacity.
			///
			/// \param capacity Capacity to assign to this buffer.
			/// \param flag Static or dynamic data buffer.
			///
			IndexBuffer(const unsigned int capacity, const StorageFlag flag) noexcept;

			///
			/// Move constructor.
			///
			IndexBuffer(IndexBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			IndexBuffer& operator=(IndexBuffer&&) noexcept;

			///
			/// Destructor.
			///
			~IndexBuffer() noexcept;

			///
			/// Create index buffer object.
			///
			/// \param indices Indices to assign.
			/// \param flag Static or dynamic data buffer.
			///
			void create(std::span<unsigned int> indices, const StorageFlag flag);

			///
			/// Clear all data from IBO.
			///
			void clear() noexcept;

			///
			/// \brief Destroy Index Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Integer.
			///
			[[nodiscard]] int index_count() const noexcept;

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
			/// Count of indices in array.
			///
			unsigned int m_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif