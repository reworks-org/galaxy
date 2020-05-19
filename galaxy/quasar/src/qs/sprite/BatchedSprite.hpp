///
/// BatchedSprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QS_BATCHEDSPRITE_HPP_
#define QS_BATCHEDSPRITE_HPP_

#include <protostar/math/Rect.hpp>

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
		/// So update() does not need to be exposed publicly.
		///
		friend class Renderer;
	public:
		///
		/// Constructor.
		///
		BatchedSprite() noexcept;

		///
		/// Destructor.
		///
		virtual ~BatchedSprite() noexcept = default;

		///
		/// Sets the texture region for the batched sprite.
		///
		/// \param region Region defined on the texture.
		/// \param zlevel zLevel of sprite.
		///
		void load(const protostar::Rect<float>& region, const unsigned int zlevel) noexcept;

		///
		/// Sets up data.
		///
		void create() noexcept;

		///
		/// Sets up data. Specify position on creation.
		///
		/// \param x Custom x pos.
		/// \param y Custom y pos.
		///
		void create(const float x, const float y) noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		const float getOpacity() const noexcept;

		///
		/// Get z-level of vertexs.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getZLevel() const noexcept;

		///
		/// Get texture region.
		///
		/// \return Float reference.
		///
		protostar::Rect<float>& getRegion() noexcept;

	private:
		///
		/// Opacity of BatchedSprite.
		///
		float m_opacity;

		///
		/// Z-Level
		///
		unsigned int m_zLevel;

		///
		/// Region of texture used.
		///
		protostar::Rect<float> m_region;
	};
}

#endif