//
//  VFS.cpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "VFS.hpp"

namespace re
{
	void VFS::setBasePath(const std::string& path)
	{
		m_basePath = path + "/";
	}

	std::string VFS::retrieve(const std::string& fileName) const
	{
		return m_basePath + fileName;
	}

	std::string VFS::getBasePath() const
	{
		return m_basePath;
	}
}
