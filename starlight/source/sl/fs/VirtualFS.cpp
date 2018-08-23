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
		// Make sure PhysFS is initialized properly.
		if (!PHYSFS_init(nullptr))
		{
			LOG_S(FATAL) << "PhysFS did not initialize. Error: " << PHYSFS_getLastError();
		}

		// This means all of allegros file loading methods will use physfs' virtual filesystem instead.
		al_set_physfs_file_interface();
	}

	VirtualFS::VirtualFS(const std::vector<std::string>& archives)
	{
		// Make sure PhysFS is initialized properly.
		if (!PHYSFS_init(nullptr))
		{
			LOG_S(FATAL) << "PHYSFS ERROR: " << PHYSFS_getLastError();
		}

		// This means all of allegros file loading methods will use physfs' virtual filesystem instead.
		al_set_physfs_file_interface();

		// Call mount on each archive (or directory).
		for (auto& archive : archives)
		{
			mount(archive);
		}
	}

	VirtualFS::~VirtualFS()
	{
		// Clean up physfs.
		PHYSFS_deinit();
	}

	void VirtualFS::mount(const std::string& archive)
	{
		// Try to mount the archive to the VFS, else log error.
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			LOG_S(ERROR) << "Cannot load: " << archive << " | " << PHYSFS_getLastError();
		}
	}

	void VirtualFS::setWriteDir(const std::string& dir)
	{
		// Try to set the write dir for the VFS, else log error.
		if (!PHYSFS_setWriteDir(dir.c_str()))
		{
			LOG_S(ERROR) << "Cannot set write dir: " << dir << "| " << PHYSFS_getLastError();
		}
	}

	ALLEGRO_FILE* VirtualFS::open(const std::string& file)
	{
		// Open the file in the vfs, making sure it exists first.
		ALLEGRO_FILE* f = nullptr;
		if (PHYSFS_exists(file.c_str()))
		{
			// Opened only for reading because we don't want to allow writing to assets from the game.
			f = al_fopen(file.c_str(), "r");
		}
		else
		{
			LOG_S(ERROR) << "Tried to open a file that does not exist: " << file << ". " << PHYSFS_getLastError();
		}

		if (!f)
		{
			LOG_S(ERROR) << "Failed to open: " << file << ". " << PHYSFS_getLastError();
		}

		return f;
	}

	std::string VirtualFS::openAsString(const std::string& file)
	{
		// This is VirtualFS::open() not some random C method.
		ALLEGRO_FILE* f = open(file);

		// Data is copied into a string, then destroyed, so the user does not have to worry
		// about cleaning up buffers or some such.

		// Create buffer with room for null-termination char.
		auto size = al_fsize(f);
		char* buff = new char[size + 1];

		// Ensure null-terminated.
		buff[size] = '\0';

		// Read into buffer.
		al_fread(f, buff, size);

		// Copy into string.
		std::string str = buff;

		// Cleanup buffer and file.
		delete[] buff;
		al_fclose(f);

		// Then we return the data in string format.
		return str;
	}

	bool VirtualFS::writeToFile(const std::string& file, const char* data)
	{
		bool success = false;

		// Open file for writing, making sure it is successful.
		PHYSFS_File* f = PHYSFS_openWrite(file.c_str());
		if (!f)
		{
			LOG_S(ERROR) << "Could not open " << file << " for writing! | " << PHYSFS_getLastError();
		}
		else
		{
			// Make sure file is not going to cause an overflow.
			auto len = strlen(data);
			if (len > UINT_MAX)
			{
				LOG_S(ERROR) << "Tried to write data file that is too large! File Name: " << file;
			}
			else
			{
				// Write data, checking for success, and making sure length is the correct type.
				if (PHYSFS_write(f, data, static_cast<PHYSFS_uint32>(len), 1) < 1)
				{
					LOG_S(ERROR) << "Failed to write data to file " << file << " | " << PHYSFS_getLastError();
				}
				else
				{
					success = true;
				}
			}
			
			PHYSFS_close(f);
		}

		return success;
	}

	bool VirtualFS::has(const std::string& file)
	{
		if (PHYSFS_exists(file.c_str()) != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}