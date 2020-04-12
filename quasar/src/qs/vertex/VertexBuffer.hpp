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
		/// \param quadCount Number of quads being created. This is only for DYNAMIC_DRAW.
		///
		void create(const VertexStorage& vertexs, const qs::BufferType bufferType, const unsigned int quadCount = 1);

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