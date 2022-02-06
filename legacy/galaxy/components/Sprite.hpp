///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// A sprite is a texture with vertex data.
		///
		class Sprite final : public graphics::Texture, public fs::Serializable
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
			/// \param layer Rendering layer.
			/// \param tex_x Optional texture x pos.
			/// \param tex_y Optional texture y pos.
			///
			void create(std::string_view layer, const float tex_x = 0.0f, const float tex_y = 0.0f);

			///
			/// Creates the internal vertex array, with a clipped w/h.
			///
			/// \param layer Rendering layer.
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(std::string_view layer, const float width, const float height);

			///
			/// Creates the internal vertex array, with a clipped rect.
			///
			/// \param layer Rendering layer.
			/// \param x Texture x pos.
			/// \param y Texture y pos.
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(std::string_view layer, const float x, const float y, const float width, const float height);

			///
			/// Bind sprite.
			///
			void bind() noexcept override;

			///
			/// Unbind sprite.
			///
			void unbind() noexcept override;

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const std::uint8_t.
			///
			[[nodiscard]] const std::uint8_t get_opacity() const noexcept;

			///
			/// Get rendering layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Gets GL VAO.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int vao() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

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
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Opacity.
			///
			std::uint8_t m_opacity;

			///
			/// Rendering Layer.
			///
			std::string m_layer;
		};
	} // namespace components
} // namespace galaxy

#endif