///
/// SpriteVertex.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITEVERTEX_HPP_
#define QUASAR_SPRITEVERTEX_HPP_

#include <glm/vec2.hpp>
#include <protostar/system/Concepts.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single SpriteVertex of a primitive.
	///
	struct SpriteVertex final
	{
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
		/// \param opacity Opacity of sprite.
		///
		SpriteVertex(const float x, const float y, const float u, const float v, const float opacity) noexcept;

		///
		/// Copy constructor.
		///
		SpriteVertex(const SpriteVertex&);

		///
		/// Move constructor.
		///
		SpriteVertex(SpriteVertex&&);

		///
		/// Copy assignment operator.
		///
		SpriteVertex& operator=(const SpriteVertex&);

		///
		/// Move assignment operator.
		///
		SpriteVertex& operator=(SpriteVertex&&);

		///
		/// Destructor.
		///
		~SpriteVertex() noexcept = default;

		///
		/// Position of SpriteVertex.
		///
		glm::vec2 m_pos;

		///
		/// SpriteVertex texels.
		///
		glm::vec2 m_texels;

		///
		/// Opacity.
		///
		float m_opacity;
	};
} // namespace qs

#endif