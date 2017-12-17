//
//  FontManager.hpp
//  rework
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_FONTMANAGER_HPP_
#define REWORK_FONTMANAGER_HPP_

#include <unordered_map>

#include <allegro5/allegro_font.h>

#include "re/types/Service.hpp"

namespace re
{
	class FontManager : public Service
	{
	public:
		///
		/// \brief Construct FontManager and add fonts.
		///
		/// You can have multiple of the same font file provided the id and size are different.
		/// E.g. "Arial12" or "Arial14".
		///
		/// \param script Script file containing font data to parse.
		///
		FontManager(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~FontManager() override;

		
		///
		/// Retrieve a font.
		///
		/// \param id The key of the font to retrieve.
		///
		/// \return Returns a pointer to the font.
		///
		ALLEGRO_FONT* get(const std::string& id);

	private:
		std::unordered_map<std::string, ALLEGRO_FONT*> m_fontMap;
	};
}

#endif
