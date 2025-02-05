///
/// PhysfsStream.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <physfs.h>

#include "galaxy/logging/PhysFSError.hpp"

#include "PhysfsStream.hpp"

namespace galaxy
{
	namespace fs
	{
		PhysfsStream::PhysfsStream(const std::string& filename)
			: m_file(nullptr)
		{
			open(filename);
		}

		PhysfsStream::~PhysfsStream()
		{
			close();
		}

		bool PhysfsStream::is_open() const
		{
			return m_file != nullptr;
		}

		bool PhysfsStream::open(const std::string& filename)
		{
			if (!filename.empty())
			{
				close();

				m_file = PHYSFS_openRead(filename.c_str());
				logging::physfs_check(m_file);
				return is_open();
			}
			else
			{
				return false;
			}
		}

		void PhysfsStream::close()
		{
			if (is_open())
			{
				logging::physfs_check(PHYSFS_close(m_file));
				m_file = nullptr;
			}
		}

		sf::Int64 PhysfsStream::read(void* data, sf::Int64 size)
		{
			if (!is_open())
			{
				return -1;
			}

			// PHYSFS_readBytes returns the number of bytes read or -1 on error.
			// We request to read size amount of bytes.
			return PHYSFS_readBytes(m_file, data, static_cast<PHYSFS_uint32>(size));
		}

		sf::Int64 PhysfsStream::seek(sf::Int64 position)
		{
			if (!is_open())
			{
				return -1;
			}

			// PHYSFS_seek return 0 on error and non zero on success.
			if (PHYSFS_seek(m_file, position))
			{
				return tell();
			}
			else
			{
				return -1;
			}
		}

		sf::Int64 PhysfsStream::tell()
		{
			if (!is_open())
			{
				return -1;
			}

			// PHYSFS_tell returns the offset in bytes or -1 on error just like SFML wants.
			return PHYSFS_tell(m_file);
		}

		sf::Int64 PhysfsStream::getSize()
		{
			if (!is_open())
			{
				return -1;
			}

			// PHYSFS_fileLength also the returns length of file or -1 on error just like SFML wants.
			return PHYSFS_fileLength(m_file);
		}
	} // namespace fs
} // namespace galaxy
