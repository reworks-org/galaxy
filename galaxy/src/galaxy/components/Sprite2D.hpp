///
/// Sprite2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE2D_HPP_
#define GALAXY_COMPONENTS_SPRITE2D_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Everything you need to draw a Sprite2D.
		///
		class Sprite2D final : public graphics::VertexData, public graphics::Texture, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Sprite2D() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Sprite2D(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Sprite2D(Sprite2D&&) noexcept;

			///
			/// Move assignment operator.
			///
			Sprite2D& operator=(Sprite2D&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Sprite2D() noexcept = default;

			///
			/// Creates the internal vertex array.
			///
			/// \param tex_x Optional texture x pos.
			/// \param tex_y Optional texture y pos.
			///
			void create(const float tex_x = 0.0f, const float tex_y = 0.0f);

			///
			/// Creates the internal vertex array, with a clipped w/h.
			///
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(const float width, const float height);

			///
			/// Creates the internal vertex array, with a clipped rect.
			///
			/// \param x Optional texture x pos.
			/// \param y Optional texture y pos.
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(const float x, const float y, const float width, const float height);

			///
			/// Activate Sprite2D context.
			///
			void bind() noexcept override;

			///
			/// Deactivate Sprite2D context.
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
			/// Get Sprite2D vertexs.
			///
			/// \return Const reference to std::vector of Sprite2D vertexs.
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
			Sprite2D(const Sprite2D&) = delete;

			///
			/// Copy assignment operator.
			///
			Sprite2D& operator=(const Sprite2D&) = delete;

		private:
			///
			/// Opacity of Sprite2D.
			///
			float m_opacity;

			///
			/// Texture ID.
			///
			std::string m_texture_str;

			///
			/// Sprite2D vertexs.
			///
			std::vector<graphics::SpriteVertex> m_vertexs;
		};
	} // namespace components
} // namespace galaxy

#endif