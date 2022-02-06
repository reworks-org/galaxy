///
/// Font.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_FONT_HPP_
#define GALAXY_GRAPHICS_TEXT_FONT_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/text/Character.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A font with a texture map for characters in the font.
		///
		class Font final
		{
			using CharMap = robin_hood::unordered_node_map<char, Character>;

		public:
			///
			/// Constructor.
			///
			Font() noexcept;

			///
			/// Argument constructor.
			///
			/// \param filepath Path to the font file.
			/// \param size Font size.
			///
			/// \return True if successful.
			///
			Font(std::string_view filepath, const int size);

			///
			/// Move constructor.
			///
			Font(Font&&) noexcept;

			///
			/// Move assignment operator.
			///
			Font& operator=(Font&&) noexcept;

			///
			/// Destructor.
			///
			~Font() noexcept;

			///
			/// Creates the font and sets up characters.
			///
			/// \param file Path to the font file.
			/// \param size Font size.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] const bool create(std::string_view file, const int size);

			///
			/// Get a character.
			///
			/// \param c Retrieve a character from the map.
			///
			/// \return Pointer to character object.
			///
			[[nodiscard]] Character* get_char(char c) noexcept;

			///
			/// Retrieve width of a string of text.
			///
			/// \param text Text to get width of.
			///
			[[nodiscard]] const int get_width(std::string_view text) noexcept;

			///
			/// Retrieve font height.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get font pixel size.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_pixel_size() const noexcept;

			///
			/// Get font file name.
			///
			/// \return Const std::string.
			///
			[[nodiscard]] const std::string& get_filename() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Font(const Font&) = delete;

			///
			/// Copy assignment operator.
			///
			Font& operator=(const Font&) = delete;

		private:
			///
			/// Font height cache.
			///
			int m_height;

			///
			/// Pixel size.
			///
			int m_size;

			///
			/// Font filename.
			///
			std::string m_filename;

			///
			/// Character map to lookup characters.
			///
			CharMap m_characters;
		};
	} // namespace graphics
} // namespace galaxy

#endif