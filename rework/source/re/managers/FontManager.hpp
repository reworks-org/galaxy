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
		/// Cleanup fonts.
		///
		~FontManager() override;

		///
		/// \brief Add a font to the manager.
		///
		/// You can have multiple of the same font file provided the size is different for each.
		///
		/// \param font Name of font in vfs to add.
		/// \param name Key to access font.
		/// \param size Font size to load.
		///
		void add(const std::string& font, const std::string& name, int size);

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
