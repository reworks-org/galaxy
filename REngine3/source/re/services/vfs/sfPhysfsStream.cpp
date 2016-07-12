//
//  sfPhysfsStream.cpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"

#include "sfPhysfsStream.hpp"

namespace re
{
	sfPhysfsStream::sfPhysfsStream(const std::string& fileName)
	{
		Open(fileName);
	}

	sfPhysfsStream::~sfPhysfsStream()
	{
		PHYSFS_close(m_file);
	}

	void sfPhysfsStream::Open(const std::string& fileName)
	{
		m_file = PHYSFS_openRead(fileName.c_str());
		if (!m_file)
		{
			std::string message = "Failed to load " + fileName + "using sfPhysfsStream";
			RE_LOG(LogLevel::WARNING, message);
		}
	}

	sf::Int64 sfPhysfsStream::read(void* data, sf::Int64 size)
	{
		if (!m_file)
		{
			return -1;
		}
		else
		{
			return PHYSFS_read(m_file, data, 1, static_cast<PHYSFS_uint32>(size));
		}
	}

	sf::Int64 sfPhysfsStream::seek(sf::Int64 position)
	{
		if (!m_file)
		{
			return -1;
		}
		else
		{
			if (PHYSFS_seek(m_file, position))
			{
				return tell();
			}
			else
			{
				return -1;
			}
		}
	}

	sf::Int64 sfPhysfsStream::tell()
	{
		if (!m_file)
		{
			return -1;
		}
		else
		{
			return PHYSFS_tell(m_file);
		}
	}

	sf::Int64 sfPhysfsStream::getSize()
	{
		if (!m_file)
		{
			return -1;
		}
		else
		{
			return PHYSFS_fileLength(m_file);
		}
	}
}