///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/math/Rect.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace sc
{
	namespace panel
	{
		class EntityEditor;
	} // namespace panel
} // namespace sc

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
	} // namespace systems

	namespace components
	{
		///
		/// A sprite is a texture with vertex data.
		///
		class Sprite final : public fs::Serializable, public graphics::Renderable
		{
			friend class sc::panel::EntityEditor;
			friend class systems::RenderSystem;

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
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			Sprite(Sprite* ptr);

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
			/// Creates the internal vertex array.
			///
			/// \param texture Texture file in VFS.
			/// \param layer Rendering layer.
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void create(const std::string& texture, const int layer, const float opacity);

			///
			/// Creates the internal vertex array.
			///
			/// \param texture Texture file in VFS.
			/// \param texture_rect Custom region on a texture to render from.
			/// \param layer Rendering layer.
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void create(const std::string& texture, const math::iRect& texture_rect, const int layer, const float opacity);

			///
			/// Updates texture and internal vertex array.
			///
			/// \param texture Texture file in VFS.
			///
			void update(const std::string& texture);

			///
			/// Updates texture and internal vertex array.
			///
			/// \param texture Texture file in VFS.
			/// \param texture_rect Custom region on a texture to render from.
			///
			void update(const std::string& texture, const math::iRect& texture_rect);

			///
			/// Set opacity.
			///
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void set_opacity(const float opacity);

			///
			/// Get opacity.
			///
			/// \return float. In range 0.0f - 1.0f.
			///
			[[nodiscard]] float get_opacity() const;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const;

			///
			/// Get texture ID in texture atlas.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_texture_name() const;

			///
			/// Number of instances to render.
			///
			/// \return Count for number of instances.
			///
			[[nodiscard]] int get_instances() const override;

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_mode() const override;

			///
			/// Get vertex array object.
			///
			/// \return VAO handle.
			///
			[[nodiscard]] unsigned int get_vao() const override;

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] unsigned int get_texture() const override;

			///
			/// Get index (element) buffer count.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_count() const override;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] int get_layer() const override;

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
			/// Texture Name.
			///
			std::string m_tex_name;

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

			///
			/// Opengl texture id.
			///
			unsigned int m_tex_id;

			///
			/// Object z-level for drawing.
			///
			int m_layer;
		};
	} // namespace components
} // namespace galaxy

#endif
