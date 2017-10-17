//
//  VFS.cpp
//  rework
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <physfs.h>
#include <allegro5/allegro_physfs.h>

#include "re/utils/Log.hpp"

#include "VFS.hpp"

namespace re
{
	VFS::VFS()
	{
		if (!PHYSFS_init(nullptr))
		{
			BOOST_LOG_TRIVIAL(error) << "PHYSFS ERROR: " << std::string(PHYSFS_getLastError()) << std::endl;
		}

		al_set_physfs_file_interface();
	}

	VFS::VFS(const std::string& archive)
	{
		if (!PHYSFS_init(nullptr))
		{
			BOOST_LOG_TRIVIAL(error) << "PHYSFS ERROR: " << std::string(PHYSFS_getLastError()) << std::endl;
		}

		al_set_physfs_file_interface();

		mount(archive);
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
			BOOST_LOG_TRIVIAL(error) << "Cannot load: " << archive << std::endl;
		}
	}

	ALLEGRO_FILE* VFS::open(const std::string& file, const std::string& mode)
	{
		ALLEGRO_FILE* pointer = nullptr;
		if (PHYSFS_exists(file.c_str()))
		{
			pointer = al_fopen(file.c_str(), mode.c_str());
			m_filesToClean.push_back(pointer);
		}
		else
		{
			BOOST_LOG_TRIVIAL(warning) << "Tried to open a file that does not exist!" << std::endl;
		}

		return pointer;
	}

	std::string VFS::openAsString(const std::string& file)
	{
		ALLEGRO_FILE* f = open(file, "r");
		if (!f)
		{
			BOOST_LOG_TRIVIAL(error) << "Failed to open: " << file << std::endl;
			return "";
		}

		ALLEGRO_USTR* ustr = nullptr;
		ustr = al_fget_ustr(f);
		if (!ustr)
		{
			BOOST_LOG_TRIVIAL(error) << "Failed to read string from file!" << std::endl;
			return "";
		}

		char* buff = al_cstr_dup(ustr);
		auto buffLen = strlen(buff);

		std::string str(buff, buffLen);

		al_free(buff);
		al_ustr_free(ustr);
		al_fclose(f);

		return str;
	}
}