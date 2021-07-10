///
/// BatchSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_BATCHSPRITE_HPP_
#define GALAXY_COMPONENTS_BATCHSPRITE_HPP_

#include <glm/vec2.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Vertex.hpp"
#include "galaxy/math/Rect.hpp"

namespace galaxy
{
	namespace graphics
	{
		class SpriteBatch;
	} // namespace graphics

	namespace components
	{
		///
		/// Sprite with texture data from a TextureAtlas.
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
			/// \param layer Rendering layer.
			/// \param index Texture Atlas index this batch sprite belongs to. Optional.
			///
			void create(const math::Rect<float>& region, std::string_view layer, unsigned int index = 0) noexcept;

			///
			/// Sets the texture region for the batched sprite from the texture atlas.
			///
			/// \param texture_key ID of a texture in the texture atlas.
			/// \param layer Rendering layer.
			///
			void create(std::string_view texture_key, std::string_view layer) noexcept;

			///
			/// Update sprite region.
			///
			/// \param texture_key ID of a texture in the texture atlas.
			///
			void update_region(std::string_view texture_key) noexcept;

			///
			/// Set layer.
			///
			/// \param layer Rendering layer.
			///
			void set_layer(std::string_view layer) noexcept;

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Set a custom width.
			///
			/// \param width New width to apply to texture.
			///
			void clip_width(const float width) noexcept;

			///
			/// Set a custom height.
			///
			/// \param height New height to apply to texture.
			///
			void clip_height(const float height) noexcept;

			///
			/// Get clip.
			///
			/// \return Const reference to glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_clip() const noexcept;

			///
			/// Get layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Get opacity.
			///
			/// \return Const std::uint8_t.
			///
			[[nodiscard]] const std::uint8_t get_opacity() const noexcept;

			///
			/// Get region.
			///
			/// \return Reference to a rectangle of floats.
			///
			[[nodiscard]] const math::Rect<float>& get_region() const noexcept;

			///
			/// Get texture string key.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_key() const noexcept;

			///
			/// Get texture atlas this sprite is for.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int get_atlas_index() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
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
			std::string m_key;

			///
			/// TextureAtlas index.
			///
			unsigned int m_index;

			///
			/// Region of texture used.
			///
			math::Rect<float> m_region;

			///
			/// Clipping of region.
			///
			glm::vec2 m_clip;

			///
			/// Rendering Layer.
			///
			std::string m_layer;

			///
			/// Opacity.
			///
			std::uint8_t m_opacity;
		};
	} // namespace components
} // namespace galaxy

#endif