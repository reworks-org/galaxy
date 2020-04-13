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

#include "qs/utils/Error.hpp"
#include "qs/vertex/Vertex.hpp"
#include "qs/utils/BufferType.hpp"

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
		/// You will need to call bind() before using this buffer.
		///
		/// \param bufferType Fixed or dynamic buffer.
		/// \param vertexs Vertexs to use.
		///
		void create(const VertexStorage& vertexs, const qs::BufferType bufferType);

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
		const std::vector<qs::Vertex>& getVertexs() noexcept;

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
}

#endif