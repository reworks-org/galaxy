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

#include "sl/libs/loguru/loguru.hpp"

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

	VirtualFS::VirtualFS(const std::vector<std::string>& archives)
	{
		if (!PHYSFS_init(nullptr))
		{
			LOG_S(FATAL) << "PHYSFS ERROR: " << PHYSFS_getLastError();
		}

		al_set_physfs_file_interface();

		for (auto& archive : archives)
		{
			mount(archive);
		}
	}

	VirtualFS::~VirtualFS()
	{
		PHYSFS_deinit();
	}

	void VirtualFS::mount(const std::string& archive)
	{
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			LOG_S(FATAL) << "Cannot load: " << archive << " | " << PHYSFS_getLastError();
		}
	}

	void VirtualFS::setWriteDir(const std::string& dir)
	{
		if (!PHYSFS_setWriteDir(dir.c_str()))
		{
			LOG_S(FATAL) << "Cannot set write dir: " << dir << "| " << PHYSFS_getLastError();
		}
	}

	ALLEGRO_FILE* VirtualFS::open(const std::string& file)
	{
		ALLEGRO_FILE* f = nullptr;
		if (PHYSFS_exists(file.c_str()))
		{
			f = al_fopen(file.c_str(), "r");
		}
		else
		{
			LOG_S(ERROR) << "Tried to open a file that does not exist! | " << PHYSFS_getLastError();
		}

		if (!f)
		{
			LOG_S(ERROR) << "Failed to open: " << file << " | " << PHYSFS_getLastError();
		}

		return f;
	}

	std::string VirtualFS::openAsString(const std::string& file)
	{
		ALLEGRO_FILE* f = open(file);

		// Data is copied into a string, then destroyed, so the user does not have to worry
		// about cleaning up buffers or some such.

		auto size = al_fsize(f);
		char* buff = new char[size + 1];
		buff[size] = '\0';

		al_fread(f, buff, size);

		std::string str = buff;

		delete[] buff;
		al_fclose(f);

		return str;
	}

	bool VirtualFS::writeToFile(const std::string& file, const char* data)
	{
		bool success = false;

		PHYSFS_File* f = PHYSFS_openWrite(file.c_str());
		if (!f)
		{
			LOG_S(ERROR) << "Could not open " << file << " for writing! | " << PHYSFS_getLastError();
		}
		else
		{
			if (PHYSFS_write(f, data, strlen(data), 1) < 1)
			{
				LOG_S(ERROR) << "Failed to write data to file " << file << " | " << PHYSFS_getLastError();
			}
			else
			{
				success = true;
			}

			PHYSFS_close(f);
		}

		return success;
	}
}