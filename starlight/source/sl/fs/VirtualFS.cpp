///
/// VirtualFS.cpp
/// starlight
///
/// Created by reworks on 12/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <physfs.h>
#include <allegro5/file.h>
#include <allegro5/allegro_physfs.h>

#include "loguru/loguru.hpp"

#include "VirtualFS.hpp"

namespace sl
{
	VirtualFS::VirtualFS()
	{
		if (!PHYSFS_init(nullptr))
		{
			LOG_S(FATAL) << "PHYSFS ERROR: " << PHYSFS_getLastError();
		}

		al_set_physfs_file_interface();
	}

	VirtualFS::VirtualFS(const std::string& archive)
	{
		if (!PHYSFS_init(nullptr))
		{
			LOG_S(FATAL) << "PHYSFS ERROR: " << PHYSFS_getLastError();
		}

		al_set_physfs_file_interface();

		mount(archive.c_str());
	}

	VirtualFS::~VirtualFS()
	{
		PHYSFS_deinit();
	}

	void VirtualFS::mount(const std::string& archive)
	{
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			LOG_S(FATAL) << "Cannot load: " << archive;
		}
	}

	std::string VirtualFS::openAsString(const std::string& file)
	{
		ALLEGRO_FILE* f = nullptr;
		if (PHYSFS_exists(file.c_str()))
		{
			f = al_fopen(file.c_str(), "r");
		}
		else
		{
			LOG_S(ERROR) << "Tried to open a file that does not exist!";
		}

		if (!f)
		{
			LOG_S(ERROR) << "Failed to open: " << file;
			return "";
		}

		auto size = al_fsize(f);
		char* buff = new char[size + 1];
		buff[size] = '\0';

		al_fread(f, buff, size);

		std::string str = buff;

		delete[] buff;
		al_fclose(f);

		return str;
	}
}