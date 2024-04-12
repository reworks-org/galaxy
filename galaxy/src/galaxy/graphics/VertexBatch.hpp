///
/// VertexBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SPRITEBATCH_HPP_
#define GALAXY_GRAPHICS_SPRITEBATCH_HPP_

#include "galaxy/graphics/gl/VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Batches together vertex data to reduce draw calls.
		///
		/// Usually used as part of other components, like a particle engine or map system.
		///
		class VertexBatch final
		{
		  public:
			///
			/// Constructor.
			///
			VertexBatch();

			///
			/// Move constructor.
			///
			VertexBatch(VertexBatch&&);

			///
			/// Move assignment operator.
			///
			VertexBatch& operator=(VertexBatch&&);

			///
			/// Destructor.
			///
			~VertexBatch();

			///
			/// Initialize vertex batch data.
			///
			/// \param max_quads Number of quads to allow to be batched.
			///
			void init(const int max_quads);

			///
			/// Buffer some vertices into the spritebatch.
			///
			/// \param vertices Vertex data to add to spritebatch buffer.
			///
			/// \return Index where the vertx data is offset from. Useful to update animations, transforms later. -1 on error.
			///
			[[maybe_unused]] int push(std::span<Vertex> vertices);

			///
			/// Sub-buffer vertex object.
			///
			/// \param index Offset to start at. 0 = first vertex group (usually a group of 4 quads).
			/// \param vertices Vertices to assign.
			///
			void sub_buffer(const unsigned int index, std::span<Vertex> vertices);

			///
			/// Clears out vertex buffer.
			///
			void clear();

			///
			/// Get vertex array object.
			///
			/// \return Reference to VAO.
			///
			[[nodiscard]] VertexArray& vao();

		  private:
			///
			/// Copy constructor.
			///
			VertexBatch(const VertexBatch&) = delete;

			///
			/// Copy assignment operator.
			///
			VertexBatch& operator=(const VertexBatch&) = delete;

		  private:
			///
			/// Vertex data.
			///
			VertexArray m_vao;

			///
			/// Maximum quads allowed.
			///
			int m_max_quads;

			///
			/// Current index.
			///
			int m_index;
		};
	} // namespace graphics
} // namespace galaxy

#endif
