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
			/// \param depth Z-Level.
			/// \param tex_x Optional texture x pos.
			/// \param tex_y Optional texture y pos.
			///
			void create(const int depth, const float tex_x = 0.0f, const float tex_y = 0.0f);

			///
			/// Creates the internal vertex array, with a clipped w/h.
			///
			/// \param depth Z-Level.
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(const int depth, const float width, const float height);

			///
			/// Creates the internal vertex array, with a clipped rect.
			///
			/// \param depth Z-Level.
			/// \param x Texture x pos.
			/// \param y Texture y pos.
			/// \param width Clipped texture width.
			/// \param height Clipped texture height.
			///
			void create_clipped(const int depth, const float x, const float y, const float width, const float height);

			///
			/// Bind sprite.
			///
			void bind() noexcept;

			///
			/// Unbind sprite.
			///
			void unbind() noexcept;

			///
			/// Set opacity.
			///
			/// \param opacity 0.0f - 1.0f.
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_opacity() const noexcept;

			///
			/// Get depth of sprite.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_depth() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int count() const noexcept;

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
			float m_opacity;
		};
	} // namespace components
} // namespace galaxy

#endif