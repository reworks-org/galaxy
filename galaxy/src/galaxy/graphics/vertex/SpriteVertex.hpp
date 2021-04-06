///
/// SpriteVertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_SPRITEVERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_SPRITEVERTEX_HPP_

#include <glm/vec2.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single SpriteVertex of a primitive.
		///
		class SpriteVertex final
		{
		public:
			///
			/// Default constructor.
			///
			SpriteVertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X pos.
			/// \param y Y pos.
			/// \param u Texel x pos.
			/// \param v Texel y pos.
			///
			SpriteVertex(const float x, const float y, const float u, const float v) noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Vec2 position.
			/// \param texels Vec2 texture coords.
			///
			SpriteVertex(const glm::vec2& pos, const glm::vec2& texels) noexcept;

			///
			/// Destructor.
			///
			~SpriteVertex() noexcept = default;

		public:
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