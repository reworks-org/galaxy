///
/// IndexBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_INDEXBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEX_INDEXBUFFER_HPP_

#include <array>
#include <span>
#include <vector>

#include <glad/glad.h>

#include "galaxy/graphics/GraphicsConcepts.hpp"

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
			/// Default constructor.
			///
			IndexBuffer();

			///
			/// Copy constructor.
			///
			IndexBuffer(const IndexBuffer&) = delete;

			///
			/// Move constructor.
			///
			IndexBuffer(IndexBuffer&&);

			///
			/// Copy assignment operator.
			///
			IndexBuffer& operator=(const IndexBuffer&) = delete;

			///
			/// Move assignment operator.
			///
			IndexBuffer& operator=(IndexBuffer&&);

			///
			/// Destroys buffer.
			///
			~IndexBuffer();

			///
			/// Create index buffer object.
			///
			/// \param indexs Index array to use.
			///
			template<is_buffer BufferType>
			void create(std::span<unsigned int> indexs);

			///
			/// Bind the current vertex buffer to current GL context.
			///
			void bind();

			///
			/// Unbind the current vertex buffer to current GL context.
			///
			void unbind();

			///
			/// Get the count of indicies in the index buffer.
			///
			/// \return Returns a const unsigned int.
			///
			[[nodiscard]] unsigned int count() const;

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

		template<is_buffer BufferType>
		inline void IndexBuffer::create(std::span<unsigned int> indexs)
		{
			m_count = static_cast<unsigned int>(indexs.size());
			bind();

			// Now to use constexpr to check on compile time the buffer type.
			// This is faster since we dont need to bother checking at runtime.
			// constexpr will discard the branch that is false and it wont be compiled.
			if constexpr (std::is_same<BufferType, BufferDynamic>::value)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_STATIC_DRAW);
			}

			unbind();
		}
	} // namespace graphics
} // namespace galaxy

#endif