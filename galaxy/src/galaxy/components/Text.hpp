///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TEXT_HPP_
#define GALAXY_COMPONENTS_TEXT_HPP_

#include <robin_hood.h>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Renderer2D;
	} // namespace graphics

	namespace components
	{
		///
		/// Creates text for use with OpenGL.
		///
		class Text final : public fs::Serializable
		{
			friend class graphics::Renderer2D;

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
			/// Load all data into text to prep for creation.
			///
			/// \param font Font to render text with.
			/// \param col Colour of the text.
			///
			void load(std::string_view font, const graphics::Colour& col);

			///
			/// Create the text object.
			///
			/// \param text Text to display.
			/// \param layer Rendering layer.
			///
			void create(std::string_view text, std::string_view layer);

			///
			/// Updates text.
			///
			/// \param text Text to display.
			///
			void update(std::string_view text);

			///
			/// Set colour.
			///
			/// \param col Colour of line.
			///
			void set_colour(const graphics::Colour& col) noexcept;

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Set font.
			///
			/// \param font New font in VFS.
			///
			void set_font(std::string_view font);

			///
			/// Get current colour.
			///
			/// \return Const reference to the current line colour.
			///
			[[nodiscard]] graphics::Colour& get_colour() noexcept;

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
			/// Get batch width.
			///
			/// \return Width as int.
			///
			[[nodiscard]] const int get_batch_width() const noexcept;

			///
			/// Get batch height.
			///
			/// \return Height as int.
			///
			[[nodiscard]] const int get_batch_height() const noexcept;

			///
			/// Get rendering layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int count() const noexcept;

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
			/// Get current text.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_text() const noexcept;

			///
			/// Get font id string.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_font_id() const noexcept;

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

			///
			/// Character batch.
			///
			struct CharacterBatch final
			{
				BatchSprite m_sprite;
				Transform2D m_transform;
			};

		private:
			///
			/// Rendering layer.
			///
			std::string m_layer;

			///
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;

			///
			/// Fontmap width cache.
			///
			int m_fontmap_width;

			///
			/// Fontmap height cache.
			///
			int m_fontmap_height;

			///
			/// Colour of the text.
			///
			graphics::Colour m_colour;

			///
			/// Spritebatch.
			///
			graphics::SpriteBatch m_batch;

			///
			/// Character <-> batched sprite hashmap.
			///
			robin_hood::unordered_map<unsigned int, CharacterBatch> m_batch_data;

			///
			/// Font ID.
			///
			std::string m_font_str;

			///
			/// Text ID.
			///
			std::string m_text_str;
		};
	} // namespace components
} // namespace galaxy

#endif