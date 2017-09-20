//
//  VFS.hpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_VFS_HPP_
#define RENGINE3_VFS_HPP_

#include <string>
#include <vector>

#include "re/types/Service.hpp"

namespace re
{
	class VFS : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Constructor
		*/
		VFS();

		~VFS();
		void mount(const std::string& archive);
		ALLEGRO_FILE* open(const std::string& file, const std::string& mode);

	private:
		std::vector<ALLEGRO_FILE*> m_filesToClean;
	};
}

#endif
