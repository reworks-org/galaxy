//
//  VFS.cpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <physfs.h>
#include <allegro5/allegro_physfs.h>

#include "re/utility/Log.hpp"

#include "VFS.hpp"

namespace re
{
	VFS::VFS()
	{
		if (!PHYSFS_init(nullptr))
		{
			RE_LOG(LogLevel::FATAL, std::string(PHYSFS_getLastError()), "VFS::VFS", "VFS.cpp", 19);
		}

		al_set_physfs_file_interface();
	}

	VFS::~VFS()
	{
		for (auto& elem : m_filesToClean)
		{
			al_fclose(elem);
		}
		m_filesToClean.clear();

		PHYSFS_deinit();
	}

	void VFS::mount(const std::string& archive)
	{
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			std::string msg = "Cannot load: " + archive;
			RE_LOG(LogLevel::WARNING, msg, "VFS::mount", "VFS.cpp", 33);
		}
	}

	ALLEGRO_FILE* VFS::open(const std::string& file, const std::string& mode)
	{
		ALLEGRO_FILE* pointer = nullptr;
		if (PHYSFS_exists(file.c_str())
		{
			pointer = al_fopen(file.c_str(), mode.c_str());
			m_filesToClean.push_back(pointer);
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Tried to open a file that does not exist!", "VFS::open", "VFS.cpp", 58);
		}

		return pointer;
	}
}