///
/// VertexBuffer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXBUFFER_HPP_
#define QUASAR_VERTEXBUFFER_HPP_

#include <vector>

#include "qs/utils/Meta.hpp"
#include "qs/utils/Error.hpp"
#include "qs/vertex/Vertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Custom vertex storage type.
	///
	using VertexStorage = std::vector<qs::Vertex>;
	using VertexQuadStorage = std::vector<std::array<qs::Vertex, 4>>;

	///
	/// Abstraction for OpenGL vertex buffer objects.
	///
	class VertexBuffer final
	{
	public:
		///
		/// Default constructor.
		///
		VertexBuffer() noexcept;

		///
		/// \brief Create vertex buffer object.
		///
		/// Can throw exception(s).
		/// This function only differs in that the buffer type is evaluated at compile time.
		///
		/// \param vertexs Vertexs to use.
		///
		template<typename BufferType>
		void create(const VertexStorage& vertexs) noexcept;

		///
		/// Destroys buffer.
		///
		~VertexBuffer() noexcept;

		///
		/// Bind the current vertex buffer to current GL context.
		///
		void bind() noexcept;

		///
		/// Unbind the current vertex buffer to current GL context.
		///
		void unbind() noexcept;

		///
		/// Get vertex storage.
		///
		/// \return Reference to std::vector.
		///
		VertexStorage& getVertexs() noexcept;

		///
		/// Get OpenGL handle.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getID() const noexcept;

	private:
		///
		/// ID returned by OpenGL when generating buffer.
		///
		unsigned int m_id;

		///
		/// Stores vertex buffer cpu side.
		///
		VertexStorage m_vertexStorage;
	};

	template<typename BufferType>
	inline void VertexBuffer::create(const VertexStorage& vertexs) noexcept
	{
		// If not one of the two buffer type structs, throw compile-time assert.
		static_assert(std::is_same<BufferType, qs::BufferTypeDynamic>::value || std::is_same<BufferType, qs::BufferTypeStatic>::value);

		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<BufferType, qs::BufferTypeDynamic>::value)
		{
			m_vertexStorage = vertexs;
			glBufferData(GL_ARRAY_BUFFER, m_vertexStorage.size() * sizeof(qs::Vertex), nullptr, GL_DYNAMIC_DRAW);
		}
		else if constexpr (std::is_same<BufferType, qs::BufferTypeStatic>::value)
		{
			m_vertexStorage = vertexs;
			glBufferData(GL_ARRAY_BUFFER, m_vertexStorage.size() * sizeof(qs::Vertex), m_vertexStorage.data(), GL_STATIC_DRAW);
		}
		else
		{
			throw std::runtime_error("How did you even get here???");
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

#endif