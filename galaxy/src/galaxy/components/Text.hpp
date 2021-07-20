///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TEXT_HPP_
#define GALAXY_COMPONENTS_TEXT_HPP_

#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Creates text for use with OpenGL.
		///
		class Text final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Text() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Text(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Text(Text&&) noexcept;

			///
			/// Move assignment operator.
			///
			Text& operator=(Text&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Text() noexcept;

			///
			/// Create the text object.
			///
			/// \param font Font to render text with.
			/// \param col Colour of the text.
			/// \param text Text to display.
			/// \param layer Rendering layer.
			///
			void create(std::string_view font, const graphics::Colour& col, std::string_view text, std::string_view layer);

			///
			/// Updates text.
			///
			/// \param text Text to display.
			///
			void update(std::string_view text);

			///
			/// Set colour.
			///
			/// \param r Red colour. 0 - 255.
			/// \param g Green colour. 0 - 255.
			/// \param b Blue colour. 0 - 255.
			///
			void set_colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept;

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get current colour.
			///
			/// \return Const reference to the current line colour.
			///
			[[nodiscard]] graphics::Colour& get_colour() noexcept;

			///
			/// Get rendering layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Get text width.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get text height.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

			///
			/// Get the GL VAO.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int vao() const noexcept;

			///
			/// Get the gl texture.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int gl_texture() const noexcept;

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
			Text(const Text&) = delete;

			///
			/// Copy assignment operator.
			///
			Text& operator=(const Text&) = delete;

		private:
			///
			/// Font ID.
			///
			std::string m_font_id;

			///
			/// Colour of the text.
			///
			graphics::Colour m_colour;

			///
			/// Text.
			///
			std::string m_text;

			///
			/// Rendering layer.
			///
			std::string m_layer;

			///
			/// Glyph shader.
			///
			graphics::Shader m_shader;

			///
			/// Glyph VBO.
			///
			unsigned int m_glyph_vbo;

			///
			/// Glyph VAO.
			///
			unsigned int m_glyph_vao;

			///
			/// Text Texture.
			///
			graphics::RenderTexture m_text_texture;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_text_vao;

			///
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;
		};
	} // namespace components
} // namespace galaxy

#endif