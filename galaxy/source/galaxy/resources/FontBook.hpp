///
/// FontBook.hpp
/// galaxy
///
/// Created by reworks on 22/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FONTBOOK_HPP_
#define GALAXY_FONTBOOK_HPP_

#include <allegro5/allegro_font.h>

#include "galaxy/types/ResourceCache.hpp"

galaxy
{
	///
	/// Manages all the fonts the library uses.
	///
	class FontBook final : public ResourceCache<ALLEGRO_FONT*>
	{
	public:
		///
		/// \brief Construct FontManager and add fonts.
		///
		/// You can have multiple of the same font file provided the id and size are different.
		/// E.g. "Arial12" or "Arial14". Size should be pixel size. Appends font folder from config for you to ttf files.
		///
		/// \param script Script file containing font data to parse.
		///
		explicit FontBook(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~FontBook() noexcept override;

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		FontBook() = delete;
	};
}

#endif