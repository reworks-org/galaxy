///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"
#include "galaxy/math/Rect.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// A sprite is a texture with vertex data.
		///
		class Sprite final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			Sprite();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Sprite(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Sprite(Sprite&&);

			///
			/// Move assignment operator.
			///
			Sprite& operator=(Sprite&&);

			///
			/// Destructor.
			///
			virtual ~Sprite();

			///
			/// Sets the texture and vertex data.
			///
			/// \param texture Texture file in VFS.
			///
			void set_texture(const std::string& texture);

			///
			/// Sets the texture and vertex data.
			///
			/// \param texture Texture file in VFS.
			/// \param rect Custom region on a texture to render from.
			///
			void set_texture(const std::string& texture, const math::iRect& rect);

			///
			/// Get texture.
			///
			/// \return Pointer. DO NOT STORE THIS.
			///
			[[nodiscard]] graphics::Texture2D* get_texture();

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

		  public:
			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Texture tint.
			///
			graphics::Colour m_tint;

		  private:
			///
			/// Texture.
			///
			graphics::Texture2D* m_texture;

			///
			/// Texture debug name.
			///
			std::string m_name;
		};
	} // namespace components
} // namespace galaxy

#endif
