///
/// IndexBuffer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INDEXBUFFER_HPP_
#define QUASAR_INDEXBUFFER_HPP_

#include <array>
#include <vector>

#include "qs/utils/Meta.hpp"
#include "qs/utils/BufferType.hpp"

#include <glad/glad.h>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Custom index storage type.
	///
	using IndexStorage = std::vector<unsigned int>;

	///
	/// Abstraction for OpenGL index (element) buffer objects.
	///
	class IndexBuffer final
	{
	public:
		///
		/// Default constructor.
		///
		IndexBuffer() noexcept;

		///
		/// \brief Create index buffer object.
		///
		/// Can throw exception(s).
		/// This function only differs in that the buffer type is evaluated at compile time.
		///
		/// \param indexs Index array to use.
		///
		template<typename BufferType>
		void create(const qs::IndexStorage& indexs);

		///
		/// Create index buffer from dynamic container.
		///
		/// \param indexs Index array to use.
		/// \param bufferType Fixed or dynamic buffer.
		///
		void create(const std::vector<unsigned int>& indexs, const qs::BufferType bufferType) noexcept;

		///
		/// Destroys buffer.
		///
		~IndexBuffer() noexcept;

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
		unsigned int getCount() const;

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

	template<typename BufferType>
	inline void IndexBuffer::create(const qs::IndexStorage& indexs)
	{
		m_count = static_cast<unsigned int>(indexs.size());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

		// If not one of the two buffer type structs, throw compile-time assert.
		static_assert(std::is_same<BufferType, qs::DynamicBufferType>::value || std::is_same<BufferType, qs::StaticBufferType>::value);

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<BufferType, qs::DynamicBufferType>::value)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_DYNAMIC_DRAW);
		}
		else if constexpr (std::is_same<BufferType, qs::StaticBufferType>::value)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_STATIC_DRAW);
		}
		else
		{
			throw std::runtime_error("How did you even get here???");
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

#endif