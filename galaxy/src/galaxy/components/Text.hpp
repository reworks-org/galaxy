///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TEXT_HPP_
#define GALAXY_COMPONENTS_TEXT_HPP_

#include "galaxy/graphics/text/Font.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Renderer;
	} // namespace graphics

	namespace components
	{
		///
		/// Creates text for use with OpenGL.
		///
		class Text final
		{
			friend class graphics::Renderer;

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
			~Text() noexcept;

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
			///
			void create(std::string_view text);

			///
			/// Update the text.
			///
			/// \param text New text to display.
			///
			void update_text(std::string_view text);

			///
			/// Activate context.
			///
			void bind();

			///
			/// Deactivate context.
			///
			void unbind() noexcept;

			///
			/// Get colour of text.
			///
			/// \return Size 4 array of floats.
			///
			[[nodiscard]] std::array<float, 4> get_colour() noexcept;

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
			/// Gets the index count.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int index_count() const noexcept;

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
				BatchedSprite m_sprite;
				Transform m_transform;
			};

		private:
			///
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;

			///
			/// Colour of the text.
			///
			graphics::Colour m_colour;

			///
			/// Pointer to font used for this text.
			///
			graphics::Font* m_font;

			///
			/// Spritebatch.
			///
			graphics::SpriteBatch m_batch;

			///
			/// Character <-> batched sprite hashmap.
			///
			robin_hood::unordered_map<unsigned int, CharacterBatch> m_batch_data;
		};
	} // namespace components
} // namespace galaxy

#endif