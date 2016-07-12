//
//  sfPhysfsStream.hpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SFPHYSFSSTREAM_HPP_
#define RENGINE3_SFPHYSFSSTREAM_HPP_

#include <string>

#include <physfs/physfs.h>
#include <SFML/System/InputStream.hpp>

namespace re
{
	class sfPhysfsStream : public sf::InputStream
	{
	public:
		/*
		* IMPORTS: fileName - The name of the file to open in the VFS.
		* EXPORTS: none
		* PURPOSE: Calls the Open() functiojn using the constructor.
		*/
		sfPhysfsStream(const std::string& fileName);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the loaded file.
		*/
		~sfPhysfsStream() override;

		/*
		* IMPORTS: fileName - the file to open in the VFS.
		* EXPORTS: none
		* PURPOSE: Opens a file using PhysFS from the VFS.
		*/
		void Open(const std::string& fileName);

		sf::Int64 read(void* data, sf::Int64 size) override;
		sf::Int64 seek(sf::Int64 position) override;
		sf::Int64 tell() override;
		sf::Int64 getSize() override;
	private:
		PHYSFS_File* m_file;
	};
}

#endif