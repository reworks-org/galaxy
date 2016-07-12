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
#include <memory>

#include "re/services/Service.hpp"
#include "re/services/vfs/sfPhysfsStream.hpp"

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
		void Mount(const std::string& archive);

		/*
		* IMPORTS: archive - the folder / archive to remove.
		* EXPORTS: none
		* PURPOSE: Remove a folder or archive from the VFS.
		*/
		void UnMount(const std::string& archive);

		/*
		* IMPORTS: fileName - The name of a file in the VFS.
		* EXPORTS: none
		* PURPOSE: Returns true if the fileName exists in the VFS.
		*/
		bool DoesExist(const std::string& fileName);

		/*
		* IMPORTS: fileName - The name of the file in the VFS.
		* EXPORTS: sfPhysfsStream - A smart pointer to an SFML input stream using Physfs.
		* PURPOSE: Returns the loaded file from the VFS using an SFML input stream.
		*/
		std::unique_ptr<sfPhysfsStream> ToStream(const std::string& fileName);

		/*
		* IMPORTS: fileName - The name of the file in the VFS.
		* EXPORTS: std::string - A buffer containing the file in std::string format.
		* PURPOSE: To load the fileName from the VFS in a std::string format.
		*/
		std::string ToString(const std::string& fileName);

		/*
		* IMPORTS: fileName - The name of the file in the VFS.
		* EXPORTS: char* - A buffer containing the file in a c-style format.
		* PURPOSE: To load the fileName from the VFS in a char* format.
		*/
		char* ToBuffer(const std::string& fileName);
	};
}

#endif