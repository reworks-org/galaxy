///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/texture/Texture.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Everything you need to draw a sprite.
		///
		class Sprite final : public graphics::VertexData, public graphics::Texture, public fs::Serializable
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
			virtual ~Sprite() noexcept = default;

			///
			/// Creates the internal vertex array.
			///
			/// \param tex_x Optional texture x pos.
			/// \param tex_y Optional texture y pos.
			///
			void create(const float tex_x = 0.0f, const float tex_y = 0.0f);

			///
			/// Creates the internal vertex array, with a clipped texture.
			///
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(const float width, const float height);

			///
			/// Activate sprite context.
			///
			void bind() noexcept override;

			///
			/// Deactivate sprite context.
			///
			void unbind() noexcept override;

			///
			/// Set opacity.
			///
			/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_opacity() const noexcept;

			///
			/// Get sprite vertexs.
			///
			/// \return Const reference to std::vector of sprite vertexs.
			///
			[[nodiscard]] const std::vector<graphics::SpriteVertex>& get_vertexs() const noexcept;

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

		private:
			///
			/// Opacity of sprite.
			///
			float m_opacity;

			///
			/// Texture ID.
			///
			std::string m_texture_str;

			///
			/// Sprite vertexs.
			///
			std::vector<graphics::SpriteVertex> m_vertexs;
		};
	} // namespace components
} // namespace galaxy

#endif