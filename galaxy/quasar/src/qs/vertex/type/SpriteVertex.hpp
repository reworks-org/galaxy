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
		/// \param opacity Opacity of sprite.
		///
		SpriteVertex(const float x, const float y, const float u, const float v, const pr::from_0_to_1 auto opacity) noexcept;

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