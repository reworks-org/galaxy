///
/// SpriteBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SPRITEBATCH_HPP_
#define GALAXY_GRAPHICS_SPRITEBATCH_HPP_

#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Batches together vertex data to reduce draw calls.
		///
		/// Usually used as part of other components, like a particle engine or map system.
		///
		class SpriteBatch final
		{
		public:
			///
			/// Constructor.
			///
			SpriteBatch() noexcept;

			///
			/// Move constructor.
			///
			SpriteBatch(SpriteBatch&&) noexcept;

			///
			/// Move assignment operator.
			///
			SpriteBatch& operator=(SpriteBatch&&) noexcept;

			///
			/// Destructor.
			///
			~SpriteBatch() noexcept;

			///
			/// Initialize spritebatch buffers.
			///
			/// \param max_quads Number of quads to allow to be batched.
			///
			void init(const int max_quads);

			///
			/// Buffer some vertices into the spritebatch.
			///
			/// \param vertices Vertex data to add to spritebatch buffer.
			///
			/// \return Index where the vertx data is offset from. Useful to update animations, transforms later.
			///
			[[maybe_unused]] unsigned int push(const std::vector<Vertex>& vertices) noexcept;

			///
			/// \brief Buffer data into vertex object.
			///
			/// Make sure you call BEFORE sub_buffer or you will lose data.
			///
			void commit() noexcept;

			///
			/// Sub-buffer vertex object.
			///
			/// \param index Offset to start at. 0 = first vertex group (usually a group of 4 quads).
			/// \param vertices Vertices to assign.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices) noexcept;

			///
			/// Clears out vertex buffer.
			///
			void flush() noexcept;

			///
			/// Get vertex array object.
			///
			/// \return Const reference to VAO.
			///
			[[nodiscard]] const VertexArray& vao() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			SpriteBatch(const SpriteBatch&) = delete;

			///
			/// Copy assignment operator.
			///
			SpriteBatch& operator=(const SpriteBatch&) = delete;

		private:
			///
			/// Vertex data.
			///
			VertexArray m_vao;

			///
			/// Maximum bytes allowed.
			///
			unsigned int m_max_bytes;

			///
			/// Total bytes currently used.
			///
			unsigned int m_bytes_used;

			///
			/// Vertices to buffer.
			///
			std::vector<Vertex> m_vertices;
		};
	} // namespace graphics
} // namespace galaxy

#endif