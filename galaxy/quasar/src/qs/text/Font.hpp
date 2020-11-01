///
/// Font.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FONT_HPP_
#define QUASAR_FONT_HPP_

#include <robin_hood.h>

#include "qs/text/Character.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// A font with a texture map for characters in the font.
	///
	class Font final
	{
	public:
		///
		/// Constructor.
		///
		Font();

		///
		/// Argument constructor.
		///
		/// \param filepath Path to the font file.
		/// \param size Font size.
		///
		/// \return True if successful.
		///
		Font(std::string_view filepath, const unsigned int size);

		///
		/// Copy constructor.
		///
		Font(const Font&) = delete;

		///
		/// Move constructor.
		///
		Font(Font&&);

		///
		/// Copy assignment operator.
		///
		Font& operator=(const Font&) = delete;

		///
		/// Move assignment operator.
		///
		Font& operator=(Font&&);

		///
		/// Destructor.
		///
		~Font();

		///
		/// Loads the font and sets up characters.
		///
		/// \param filepath Path to the font file.
		/// \param size Font size.
		///
		/// \return True if successful.
		///
		bool load(std::string_view filepath, const unsigned int size);

		///
		/// Get a character.
		///
		/// \param c Retrieve a character from the map.
		///
		/// \return Pointer to character object.
		///
		[[nodiscard]] Character* get_char(char c);

		///
		/// Retrieve width of a string of text.
		///
		/// \param text Text to get width of.
		///
		[[nodiscard]] const int get_width(std::string_view text);

		///
		/// Retrieve font height.
		///
		/// \return Const integer.
		///
		[[nodiscard]] const int get_height() const;

	private:
		///
		/// Get the height of the font.
		///
		int m_height;

		///
		/// Character map to lookup characters.
		///
		robin_hood::unordered_map<char, Character> m_characters;
	};
} // namespace qs

#endif