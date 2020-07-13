///
/// SpriteBatchComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITEBATCHCOMPONENT_HPP_
#define GALAXY_SPRITEBATCHCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <protostar/math/Rect.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component for a spritebatch.
	///
	class SpriteBatchComponent final
	{
		friend class RenderSystem;
	public:
		///
		/// Constructor.
		///
		SpriteBatchComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SpriteBatchComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		virtual ~SpriteBatchComponent() noexcept = default;

		///
		/// Sets the texture region for the batched sprite.
		///
		/// \param region Region defined on the texture.
		/// \param zlevel zLevel of sprite.
		///
		void create(const pr::Rect<float>& region, const unsigned int zlevel) noexcept;

		///
		/// Set opacity.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Set z-level.
		///
		/// \param level New z-level of sprite.
		///
		void setZLevel(const unsigned int level) noexcept;

		///
		/// Update texquad region.
		///
		/// \param x New x position.
		/// \param y New y position.
		/// \param w Optional. Width. Will not set unless provided.
		/// \param h Optional. Height. Will not set unless provided.
		///
		void setUpdatedRegion(float x, float y, float w = -1.0f, float h = -1.0f) noexcept;

		///
		/// Get z-level.
		///
		/// \return Const unsigned int.
		///
		const unsigned int getZLevel() const noexcept;

	private:
		///
		/// Keeps track if a batched sprite has been modified.
		/// Defaults to false.
		///
		bool m_isDirty;

		///
		/// Opacity of BatchedSprite.
		///
		float m_opacity;

		///
		/// Z-Level
		///
		unsigned int m_zLevel;

		///
		/// Offset of vertexs in a spritebatch.
		///
		unsigned int m_offset;

		///
		/// Region of texture used.
		///
		pr::Rect<float> m_region;
	};
}

#endif