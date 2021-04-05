///
/// BatchSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_BATCHSPRITE_HPP_
#define GALAXY_COMPONENTS_BATCHSPRITE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		class SpriteBatch;
	} // namespace graphics

	namespace components
	{
		///
		/// Quad with a texture ID used for drawing a sprite in a spritebatch.
		///
		class BatchSprite final : public fs::Serializable
		{
			friend class graphics::SpriteBatch;

		public:
			///
			/// Constructor.
			///
			BatchSprite() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			BatchSprite(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			BatchSprite(BatchSprite&&) noexcept;

			///
			/// Move assignment operator.
			///
			BatchSprite& operator=(BatchSprite&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~BatchSprite() noexcept;

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
			/// Get sprite vertexs.
			///
			/// \return Const reference to std::vector of sprite position vertexs.
			///
			[[nodiscard]] const std::vector<glm::vec2>& get_vertexs() const noexcept;

			///
			/// Get texture string id.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_tex_id() const noexcept;

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
			BatchSprite(const BatchSprite&) = delete;

			///
			/// Copy assignment operator.
			///
			BatchSprite& operator=(const BatchSprite&) = delete;

		private:
			///
			/// Texture region ID.
			///
			std::string m_id;

			///
			/// Opacity of BatchSprite.
			///
			float m_opacity;

			///
			/// Region of texture used.
			///
			graphics::fRect m_region;

			///
			/// Custom width/height.
			///
			glm::vec2 m_custom_wh;

			///
			/// Set by spritebatch.
			///
			unsigned int m_offset;

			///
			/// Set by spritebatch.
			///
			int m_z_level;

			///
			/// Set by spritebatch.
			///
			std::vector<glm::vec2> m_vertexs;
		};
	} // namespace components
} // namespace galaxy

#endif