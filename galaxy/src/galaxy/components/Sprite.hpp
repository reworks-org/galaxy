///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// A sprite is a texture with vertex data.
		///
		class Sprite final : public graphics::Renderable, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Sprite() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Sprite(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Sprite(Sprite&&) noexcept;

			///
			/// Move assignment operator.
			///
			Sprite& operator=(Sprite&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Sprite() noexcept;

			///
			/// Creates the internal vertex array.
			///
			/// \param texture Texture file in VFS.
			/// \param layer Rendering layer.
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void create(const std::string& texture, const int layer, const float opacity = 1.0f);

			///
			/// Updates texture and internal vertex array.
			///
			/// \param texture Texture file in VFS.
			///
			void update(const std::string& texture);

			///
			/// Set opacity.
			///
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return float. In range 0.0f - 1.0f.
			///
			[[nodiscard]] float get_opacity() const noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const noexcept;

			///
			/// Get texture ID in texture atlas.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& texture_id() const noexcept;

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
			Sprite(const Sprite&) = delete;

			///
			/// Copy assignment operator.
			///
			Sprite& operator=(const Sprite&) = delete;

			///
			/// Configure renderable.
			///
			void configure() noexcept override;

		private:
			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Texture Name.
			///
			std::string m_texture;

			///
			/// Sprite opacity.
			///
			float m_opacity;

			///
			/// Cached texture width.
			///
			float m_width;

			///
			/// Cached texture height.
			///
			float m_height;
		};
	} // namespace components
} // namespace galaxy

#endif