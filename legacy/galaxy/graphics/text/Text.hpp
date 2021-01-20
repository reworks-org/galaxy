///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_TEXT_HPP_
#define GALAXY_GRAPHICS_TEXT_TEXT_HPP_

#include "galaxy/graphics/sprite/SpriteBatch.hpp"
#include "galaxy/graphics/text/Font.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Creates text for use with OpenGL.
		///
		class Text final : public Transform
		{
		public:
			///
			/// Constructor.
			///
			Text();

			///
			/// Copy constructor.
			///
			Text(const Text&) = delete;

			///
			/// Copy assignment operator.
			///
			Text& operator=(const Text&) = delete;

			///
			/// Destructor.
			///
			~Text();

			///
			/// Load all data into text to prep for creation.
			///
			/// \param font Font to render text with.
			/// \param col Colour of the text.
			///
			void load(Font* font, const graphics::Colour& col);

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
			void unbind();

			///
			/// Get colour of text.
			///
			/// \return Size 4 array of floats.
			///
			[[nodiscard]] std::array<float, 4> get_colour();

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_height() const;

			///
			/// Get batch width.
			///
			/// \return Width as int.
			///
			[[nodiscard]] const int get_batch_width() const;

			///
			/// Get batch height.
			///
			/// \return Height as int.
			///
			[[nodiscard]] const int get_batch_height() const;

			///
			/// Gets the index count.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int index_count() const;

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
			Font* m_font;

			///
			/// Spritebatch.
			///
			SpriteBatch m_batch;

			///
			/// Character <-> batched sprite hashmap.
			///
			robin_hood::unordered_map<unsigned int, BatchedSprite> m_batch_data;
		};
	} // namespace graphics
} // namespace galaxy

#endif