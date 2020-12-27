///
/// SpriteVertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_TYPE_SPRITEVERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_TYPE_SPRITEVERTEX_HPP_

#include <glm/vec2.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single SpriteVertex of a primitive.
		///
		struct SpriteVertex final
		{
			///
			/// Default constructor.
			///
			SpriteVertex();

			///
			/// Argument constructor.
			///
			/// \param x X pos.
			/// \param y Y pos.
			/// \param u Texel x pos.
			/// \param v Texel y pos.
			///
			SpriteVertex(const float x, const float y, const float u, const float v);

			///
			/// Destructor.
			///
			~SpriteVertex() = default;

			///
			/// Position of SpriteVertex.
			///
			glm::vec2 m_pos;

			///
			/// SpriteVertex texels.
			///
			glm::vec2 m_texels;
		};
	} // namespace graphics
} // namespace galaxy

#endif