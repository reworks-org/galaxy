///
/// BatchedSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_BATCHEDSPRITE_HPP_
#define GALAXY_COMPONENTS_BATCHEDSPRITE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/texture/TextureInfo.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		class SpriteBatch;
	}

	namespace components
	{
		///
		/// A sprite that does not contain a texture. Useful for when working with batches and texture atlas'.
		///
		class BatchedSprite final
		{
			friend class graphics::SpriteBatch;

		public:
			///
			/// Constructor.
			///
			BatchedSprite() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			BatchedSprite(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			BatchedSprite(BatchedSprite&&) noexcept;

			///
			/// Move assignment operator.
			///
			BatchedSprite& operator=(BatchedSprite&&) noexcept;

			///
			/// Destructor.
			///
			~BatchedSprite() noexcept;

			///
			/// Sets the texture region for the batched sprite from the texture atlas.
			///
			/// \param texture_atlas_id ID of a texture in the texture atlas.
			/// \param opacity Opacity of the batched sprite.
			///
			void create(std::string_view texture_atlas_id, float opacity = 1.0f);

			///
			/// Update texture info in the batched sprite.
			///
			/// \param id ID of a texture in the texture atlas.
			///
			void update(std::string_view id);

			///
			/// Set opacity.
			///
			/// Only affects the currently active quad.
			///
			/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float opacity() const noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get texture info.
			///
			/// \return Pointer to batched sprite texture info.
			///
			[[nodiscard]] graphics::TextureInfo* const tex_info() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			BatchedSprite(const BatchedSprite&) = delete;

			///
			/// Copy assignment operator.
			///
			BatchedSprite& operator=(const BatchedSprite&) = delete;

		private:
			///
			/// Opacity of BatchedSprite.
			///
			float m_opacity;

			///
			/// Info of texture from atlas.
			///
			graphics::TextureInfo* m_info;

			///
			/// Set by spritebatch.
			///
			unsigned int m_offset;

			///
			/// Set by spritebatch.
			///
			int m_z_level;
		};
	} // namespace components
} // namespace galaxy

#endif