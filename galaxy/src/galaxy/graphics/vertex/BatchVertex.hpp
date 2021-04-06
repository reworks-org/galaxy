///
/// BatchVertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_BATCHVERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_BATCHVERTEX_HPP_

#include <glm/vec2.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single BatchVertex of a primitive.
		///
		class BatchVertex final
		{
		public:
			///
			/// Default constructor.
			///
			BatchVertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X pos.
			/// \param y Y pos.
			/// \param u Texel x pos.
			/// \param v Texel y pos.
			/// \param opacity 1.0f = Opaque, 0.0f = Transparent.
			///
			BatchVertex(const float x, const float y, const float u, const float v, const float opacity) noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Vec2 position.
			/// \param texels Vec2 texture coords.
			/// \param opacity 1.0f = Opaque, 0.0f = Transparent.
			///
			BatchVertex(const glm::vec2& pos, const glm::vec2& texels, const float opacity) noexcept;

			///
			/// Destructor.
			///
			~BatchVertex() noexcept = default;

		public:
			///
			/// Position of BatchVertex.
			///
			glm::vec2 m_pos;

			///
			/// BatchVertex texels.
			///
			glm::vec2 m_texels;

			///
			/// Opacity.
			///
			float m_opacity;
		};
	} // namespace graphics
} // namespace galaxy

#endif