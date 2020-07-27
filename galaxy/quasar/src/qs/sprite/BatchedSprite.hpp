///
/// BatchedSprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QS_BATCHEDSPRITE_HPP_
#define QS_BATCHEDSPRITE_HPP_

#include <protostar/graphics/Rect.hpp>

#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// A sprite that does not contain a texture. Useful for when working with batches and texture atlas'.
	///
	class BatchedSprite final : public qs::Transform
	{
		///
		/// Allows access to internals that should not be publicly exposed.
		///
		friend class SpriteBatch;

	public:
		///
		/// Constructor.
		///
		BatchedSprite();

		///
		/// Destructor.
		///
		~BatchedSprite() noexcept = default;

		///
		/// Sets the texture region for the batched sprite.
		///
		/// \param region Region defined on the texture.
		/// \param z_level Z-Level of sprite.
		///
		void create(const pr::Rect<float>& region, const pr::positive_uint auto z_level) noexcept;

		///
		/// Set opacity.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void set_opacity(const pr::from_0_to_1 auto opacity) noexcept;

		///
		/// Set z-level.
		///
		/// \param z_level New z-level of sprite.
		///
		void set_z_level(const pr::positive_uint auto z_level) noexcept;

		///
		/// Update texquad region.
		///
		/// \param x New x position.
		/// \param y New y position.
		/// \param w Optional. Width. Will not set unless provided.
		/// \param h Optional. Height. Will not set unless provided.
		///
		void update_region(float x, float y, pr::positive_float auto w = 0.0f, pr::positive_float auto h = 0.0f) noexcept;

		///
		/// Update texquad region.
		///
		/// \param quad New quad region.
		///
		void update_region(const pr::Rect<float>& quad) noexcept;

		///
		/// Get z-level.
		///
		/// \return Const unsigned int.
		///
		[[nodiscard]] const unsigned int z_level() const noexcept;

	private:
		///
		/// Keeps track if a batched sprite has been modified.
		/// Defaults to false.
		///
		bool m_dirty;

		///
		/// Opacity of BatchedSprite.
		///
		float m_opacity;

		///
		/// Z-Level
		///
		unsigned int m_z_level;

		///
		/// Offset of vertexs in a spritebatch.
		///
		unsigned int m_offset;

		///
		/// Region of texture used.
		///
		pr::Rect<float> m_region;
	};
} // namespace qs

#endif