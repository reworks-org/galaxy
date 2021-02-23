///
/// BatchedSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_BATCHEDSPRITE_HPP_
#define GALAXY_COMPONENTS_BATCHEDSPRITE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		class SpriteBatch;
	} // namespace graphics

	namespace components
	{
		///
		/// A sprite that does not contain a texture. Useful for when working with batches and texture atlas'.
		///
		class BatchedSprite final : public fs::Serializable
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
			/// Sets the texture region for the batched sprite.
			///
			/// \param region Region defined on the texture.
			/// \param opacity Opacity of the batched sprite.
			///
			void create(const graphics::fRect& region, float opacity = 1.0f);

			///
			/// Sets the texture region for the batched sprite from the texture atlas.
			///
			/// \param texture_atlas_id ID of a texture in the texture atlas.
			/// \param opacity Opacity of the batched sprite.
			///
			void create(std::string_view texture_atlas_id, float opacity = 1.0f);

			///
			/// Set region.
			///
			/// \param region Name of region in texture atlas.
			///
			void set_region(std::string_view region);

			///
			/// Set opacity.
			///
			/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Set a custom width.
			///
			/// \param width New width to apply to texture.
			///
			void set_custom_width(const float width) noexcept;

			///
			/// Set a custom height.
			///
			/// \param height New height to apply to texture.
			///
			void set_custom_height(const float height) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_opacity() const noexcept;

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
			/// Get region.
			///
			/// \return Reference to a rectangle of floats.
			///
			[[nodiscard]] const graphics::fRect& get_region() const noexcept;

			///
			/// Get region this frame occupies.
			///
			/// \return Const float rectangle.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

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
			/// Texture region ID.
			///
			std::string m_id;

			///
			/// Opacity of BatchedSprite.
			///
			float m_opacity;

			///
			/// Region of texture used.
			///
			graphics::fRect m_region;

			///
			/// Set by spritebatch.
			///
			unsigned int m_offset;

			///
			/// Set by spritebatch.
			///
			int m_z_level;

			///
			/// Custom width/height.
			///
			glm::vec2 m_custom_wh;
		};
	} // namespace components
} // namespace galaxy

#endif