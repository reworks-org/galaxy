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

#include "re/types/Service.hpp"

namespace re
{
	class VFS : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Sets up the VFS by initialising PhysFS.
		*/
		VFS();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up VFS by cleaning up PhysFS.
		*/
		~VFS() override;

		/*
		* IMPORTS: archive - A folder or archive. Supports .zip & .7z formats.
		* EXPORTS: none
		* PURPOSE: Adds a folder or 7z/zip archive to the virtual file system.
		*/
		void mount(const std::string& archive);

		/*
		* IMPORTS: archive - the folder / archive to remove.
		* EXPORTS: none
		* PURPOSE: Remove a folder or archive from the VFS.
		*/
		void unMount(const std::string& archive);

		/*
		* IMPORTS: fileName - The name of a file in the VFS.
		* EXPORTS: none
		* PURPOSE: Returns true if the fileName exists in the VFS.
		*/
		bool doesExist(const std::string& fileName);

		/*
		* IMPORTS: fileName - The name of the file in the VFS.
		* EXPORTS: std::string - A buffer containing the file in std::string format.
		* PURPOSE: To load the fileName from the VFS in a std::string format.
		*/
		std::string toString(const std::string& fileName);

		/*
		* IMPORTS: fileName - The name of the file in the VFS.
		* EXPORTS: char* - A buffer containing the file in a c-style format.
		* PURPOSE: To load the fileName from the VFS in a char* format.
		*/
		char* toBuffer(const std::string& fileName);
        
        /*
        * IMPORTS: fileName - The name of the file in the VFS. data - text to write.
        * EXPORTS: none
        * PURPOSE: To write out data to a file in the vfs.
        */
        void writeToFile(const std::string& fileName, const std::string& data);

		/*
		* IMPORTS: none
		* EXPORTS: std::string to the base directory of the application.
		* PURPOSE: Get path of application.
		*/
		std::string baseDir() const;
	};
}

#endif
