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
	class BatchedSprite : public qs::Transform
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
		/// Copy constructor.
		///
		BatchedSprite(const BatchedSprite&) noexcept = delete;

		///
		/// Move constructor.
		///
		BatchedSprite(BatchedSprite&&);

		///
		/// Copy assignment operator.
		///
		BatchedSprite& operator=(const BatchedSprite&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		BatchedSprite& operator=(BatchedSprite&&);

		///
		/// Destructor.
		///
		virtual ~BatchedSprite() noexcept = default;

		///
		/// Sets the texture region for the batched sprite.
		///
		/// \param region Region defined on the texture.
		/// \param z_level Z-Level of sprite.
		///
		void create(const pr::Rect<float>& region, const unsigned int z_level) noexcept;

		///
		/// Set opacity.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void set_opacity(const float opacity) noexcept;

		///
		/// Set z-level.
		///
		/// \param z_level New z-level of sprite.
		///
		void set_z_level(const unsigned int z_level) noexcept;

		///
		/// Update texquad region.
		///
		/// \param x New x position.
		/// \param y New y position.
		/// \param w Width. Will not set unless > 0.
		/// \param h Height. Will not set unless > 0.
		///
		void update_region(float x, float y, float w, float h) noexcept;

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

	protected:
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