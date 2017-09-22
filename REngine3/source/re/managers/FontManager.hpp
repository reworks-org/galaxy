//
//  FontManager.hpp
//  REngine3
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_FONTMANAGER_HPP_
#define RENGINE3_FONTMANAGER_HPP_

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
		/// Add a font to the manager.
		///
		/// \param file Name of file in vfs to add.
		/// \param id Key to access font.
		///
		void add(const std::string& file, const std::string& id);

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
