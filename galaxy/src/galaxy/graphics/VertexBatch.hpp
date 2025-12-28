///
/// VertexBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXBATCH_HPP_
#define GALAXY_GRAPHICS_VERTEXBATCH_HPP_

#include "galaxy/graphics/gl/VertexArray.hpp"

namespace galaxy
{
	///
	/// A group of vertices for multiple renderables batched together.
	///
	class VertexBatch
	{
	public:
		///
		/// Constructor.
		///
		/// \tparam max Max renderable objects.
		/// \tparam vertex_count Number of vertices in a single object.
		/// \tparam index_count Number of indices in a single object.
		///
		VertexBatch(const int max, const int vertex_count, const int index_count) noexcept;

		///
		/// Move constructor.
		///
		VertexBatch(VertexBatch&&) noexcept;

		///
		/// Move assignment operator.
		///
		VertexBatch& operator=(VertexBatch&&) noexcept;

		///
		/// Destructor.
		///
		~VertexBatch() noexcept;

		///
		/// Add vertex and index data to batch.
		///
		/// \param vertices List of vertices.
		/// \param indices List of indices.
		///
		void push(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;

		///
		/// Copy all data to GPU.
		///
		void flush() noexcept;

		///
		/// Bind this batchs vertex array object.
		///
		void bind() const noexcept;

		///
		/// Get the index count.
		///
		/// \return Integer.
		///
		[[nodiscard]]
		int count() const noexcept;

		///
		/// Gets index offset.
		///
		/// \return Integer as void pointer for opengl shenanigans.
		///
		[[nodiscard]]
		void* offset() noexcept;

		///
		/// Gets internal VAO.
		///
		/// \return Reference to VertexArray.
		///
		[[nodiscard]]
		VertexArray& vao() noexcept;

	private:
		///
		/// Constructor.
		///
		VertexBatch() = delete;

		///
		/// \brief Copy constructor.
		///
		/// Deleted.
		///
		VertexBatch(const VertexBatch&) = delete;

		///
		/// \brief  Copy assignment operator.
		///
		/// Deleted.
		///
		VertexBatch& operator=(const VertexBatch&) = delete;

	private:
		///
		/// Amount of vertices to flush.
		///
		int m_vertex_count;

		///
		/// Amount of indices to flush.
		///
		int m_index_count;

		///
		/// Index count after flushing.
		///
		int m_count;

		///
		/// Total number of vertices.
		///
		int m_vertex_length;

		///
		/// Total number of indices.
		///
		int m_index_length;

		///
		/// CPU side reserved vertices.
		///
		std::vector<Vertex> m_vertices;

		///
		/// CPU side reserved indices.
		///
		std::vector<unsigned int> m_indices;

		///
		/// VAO object for GPU data.
		///
		VertexArray m_vao;
	};
} // namespace galaxy

#endif
