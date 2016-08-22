//
//  VFS.cpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"

#include "VFS.hpp"

namespace re
{
	VFS::VFS()
	{
		PHYSFS_init(nullptr);
	}

	VFS::~VFS()
	{
		PHYSFS_deinit();
	}

	void VFS::Mount(const std::string& archive)
	{
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			std::string message = "Cannot load: " + archive;
			RE_LOG(LogLevel::FATAL, message);
		}
	}

	void VFS::UnMount(const std::string& archive)
	{
		if (!PHYSFS_unmount(archive.c_str()))
		{
			std::string message = "Cannot unload: " + archive;
			RE_LOG(LogLevel::WARNING, message);
		}
	}

	bool VFS::DoesExist(const std::string& fileName)
	{	
		if (PHYSFS_exists(fileName.c_str()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	sf::physfs VFS::ToStream(const std::string& fileName)
	{
		sf::physfs stream(fileName.c_str());

		return stream;
	}

	std::string VFS::ToString(const std::string& fileName)
	{
		PHYSFS_file* myfile = PHYSFS_openRead(fileName.c_str());
		PHYSFS_sint64 file_size = PHYSFS_fileLength(myfile);

		char* myBuf = new char[file_size + 1];
		myBuf[file_size] = '\0';

		PHYSFS_read(myfile, myBuf, 1, file_size);

		std::string str{ myBuf };

		delete[] myBuf;
		PHYSFS_close(myfile);

		return str;
	}

	char* VFS::ToBuffer(const std::string& fileName)
	{
		PHYSFS_file* myfile = PHYSFS_openRead(fileName.c_str());
		PHYSFS_sint64 file_size = PHYSFS_fileLength(myfile);

		char* myBuf = new char[file_size + 1];
		myBuf[file_size] = '\0';

		PHYSFS_read(myfile, myBuf, 1, file_size);

		std::string str{ myBuf };

		delete[] myBuf;
		PHYSFS_close(myfile);

#ifdef _WIN32
		return _strdup(str.c_str());
#else
		return strdup(str.c_str());
#endif
	}
}