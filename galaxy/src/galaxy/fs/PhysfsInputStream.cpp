///
/// PhysfsInputStream.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <pl/Log.hpp>
#include <physfs/physfs.h>

#include "PhysfsInputStream.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	PhysfsInputStream::PhysfsInputStream() noexcept
		:m_opened(false), m_file(nullptr)
	{
	}

	PhysfsInputStream::PhysfsInputStream(const std::string& file)
		:m_opened(false), m_file(nullptr)
	{
		if (!open(file))
		{
			PL_LOG(pl::Log::Level::FATAL, "Failed to open file with PhysfsInputStream constructor! Error: " + std::string(PHYSFS_getLastError()));
			throw std::runtime_error("");
		}
	}

	PhysfsInputStream::~PhysfsInputStream() noexcept
	{
		close();
	}

	bool PhysfsInputStream::open(const std::string& file)
	{
		bool result = true;
		
		if (PHYSFS_exists(file.c_str()) != 0)
		{
			m_file = PHYSFS_openRead(file.c_str());
			m_opened = true;
		}
		else
		{
			PL_LOG(pl::Log::Level::ERROR_, "File does not exist in PhysFS. Error: " + std::string(PHYSFS_getLastError()));
			result = false;
		}

		return result;
	}

	void PhysfsInputStream::close() noexcept
	{
		if (m_opened)
		{
			PHYSFS_close(m_file);
			m_opened = false;
			m_file = nullptr;
		}
	}

	sf::Int64 PhysfsInputStream::read(void* data, sf::Int64 size) noexcept
	{
		sf::Int64 result = 0;
		if (!m_opened)
		{
			result = -1;
		}
		else
		{
			result = PHYSFS_readBytes(m_file, data, size);
		}

		return result;
	}

	sf::Int64 PhysfsInputStream::seek(sf::Int64 position) noexcept
	{
		sf::Int64 result = 0;
		if (!m_opened)
		{
			result = -1;
		}
		else
		{
			if (PHYSFS_seek(m_file, position))
			{
				result = tell();
			}
			else
			{
				result = -1;
			}
		}

		return result;
	}

	sf::Int64 PhysfsInputStream::tell() noexcept
	{
		sf::Int64 result = 0;
		if (!m_opened)
		{
			result = -1;
		}
		else
		{
			result = PHYSFS_tell(m_file);
		}

		return result;
	}

	sf::Int64 PhysfsInputStream::getSize() noexcept
	{
		sf::Int64 result = 0;
		if (!m_opened)
		{
			result = -1;
		}
		else
		{
			result = PHYSFS_fileLength(m_file);
		}

		return result;
	}

	const bool PhysfsInputStream::isOpen() const noexcept
	{
		return m_opened;
	}
}