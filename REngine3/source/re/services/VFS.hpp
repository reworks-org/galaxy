//
//  VFS.hpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_VFS_HPP_
#define RENGINE3_VFS_HPP_

#include <string>
#include <vector>

#include <allegro5/file.h>

#include "re/types/Service.hpp"

namespace re
{
	class VFS : public Service
	{
	public:
		///
		/// Constructor.
		///
		VFS();
		
		///
		/// Destructor.
		//
		~VFS();

		///
		/// \brief Mount an archive or folder.
		///
		/// This is done by physfs so it supports the most common archive types. You can also point to a folder.
		///
		/// \param archive Path or archive to mount.
		///
		void mount(const std::string& archive);

		///
		/// Open an ALLEGRO_FILE.
		///
		/// \param file The file in the VFS to open, and the mode: "r", "w", etc.
		///
		/// \return ALLEGRO_FILE*. This is automatically cleaned up by the VFS. DO NOT MANUALLY CLEAN THIS UP.
		///
		ALLEGRO_FILE* open(const std::string& file, const std::string& mode);

		///
		/// \brief Open a file and return as a std::string.
		///
		/// Please note this string is self-contained. You don't need to free anything. Also note this only supports files that contain text!
		///
		/// \param file The file in the VFS to open.
		///
		/// \return Returns a std::string. This is a plain string containing the information.
		///
		std::string openAsString(const std::string& file);

	private:
		std::vector<ALLEGRO_FILE*> m_filesToClean;
	};
}

#endif
