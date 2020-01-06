///
/// FileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <pl/Log.hpp>
#include <physfs/physfs.h>

#include "FileSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	FileSystem::FileSystem()
	{
		// Make sure PhysFS is initialized properly.
		if (!PHYSFS_init(nullptr))
		{
			PL_LOG(pl::Log::Level::FATAL, "PhysFS did not initialize. Error: " + std::string(PHYSFS_getLastError()));
			throw std::runtime_error("");
		}
	}

	FileSystem::FileSystem(const std::vector<std::string>& archives)
	{
		// Make sure PhysFS is initialized properly.
		if (!PHYSFS_init(nullptr))
		{
			PL_LOG(pl::Log::Level::FATAL, "PhysFS did not initialize. Error: " + std::string(PHYSFS_getLastError()));
			throw std::runtime_error("");
		}

		// Call mount on each archive (or directory).
		for (auto& archive : archives)
		{
			mount(archive);
		}
	}

	FileSystem::~FileSystem()
	{
		// Clean up physfs.
		PHYSFS_deinit();
	}

	void FileSystem::setWriteDir(const std::string& dir)
	{
		// Try to set the write dir for the VFS, else log error.
		if (!PHYSFS_setWriteDir(dir.c_str()))
		{
			PL_LOG(pl::Log::Level::FATAL, "Cannot set write dir: " + dir + " | " + std::string(PHYSFS_getLastError()));
			throw std::runtime_error("");
		}
	}

	void FileSystem::mount(const std::string& archive)
	{
		// Try to mount the archive to the VFS, else log error.
		if (!PHYSFS_mount(archive.c_str(), nullptr, 1))
		{
			PL_LOG(pl::Log::Level::FATAL, "Cannot load: " + archive + " | " + std::string(PHYSFS_getLastError()));
			throw std::runtime_error("");
		}
	}

	bool FileSystem::has(const std::string& file)
	{
		return (PHYSFS_exists(file.c_str()) != 0);
	}

	const std::string FileSystem::read(const std::string& file)
	{
		std::string out = "";

		// Open file for reading, making sure it is successful.
		PHYSFS_File* fp = PHYSFS_openRead(file.c_str());
		if (!fp)
		{
			PL_LOG(pl::Log::Level::FATAL, "Could not open " + file + " for reading! | " + std::string(PHYSFS_getLastError()));
			PHYSFS_close(fp);

			throw std::runtime_error("");
		}
		else
		{
			auto size = PHYSFS_fileLength(fp);
			char* buff = new char[size + 1];
			
			// Read data, checking for success, and making sure size means full file was read.
			if (PHYSFS_readBytes(fp, buff, size) != size)
			{
				PL_LOG(pl::Log::Level::FATAL, "Failed to read data from file " + file + " | " + std::string(PHYSFS_getLastError()));
				delete[] buff;
				PHYSFS_close(fp);

				throw std::runtime_error("");
			}
			else
			{
				buff[size + 1] = '\0';
				out.assign(buff, size + 1);
			}

			delete[] buff;
		}

		PHYSFS_close(fp);

		return out;
	}

	bool FileSystem::write(const std::string& file, const void* data, const unsigned int size)
	{
		bool result = true;

		// Open file for writing, making sure it is successful.
		PHYSFS_File* fp = PHYSFS_openWrite(file.c_str());
		if (!fp)
		{
			PL_LOG(pl::Log::Level::ERROR, "Could not open " + file + " for writing! | " + std::string(PHYSFS_getLastError()));
			result = false;
		}
		else
		{
			// Write data, checking for success, and making sure size means full data was written.
			if (PHYSFS_writeBytes(fp, data, size) != size)
			{
				PL_LOG(pl::Log::Level::ERROR, "Failed to write data to file " + file + " | " + std::string(PHYSFS_getLastError()));
				result = false;
			}
		}

		// All done.
		PHYSFS_close(fp);

		return result;
	}
}