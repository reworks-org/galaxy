//
//  VFS.hpp
//  REngine3
//
//  Created by reworks on 12/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_VFS_HPP_
#define RENGINE3_VFS_HPP_

#include <string>
#include <unordered_map>

#include "re/types/Service.hpp"

namespace re
{
	class VFS : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Constructor
		*/
		VFS();

		/*
		* IMPORTS: path to main assets folder. DO NOT PUT A '/' AT THE END OF THE STRING!
		* EXPORTS: none
		* PURPOSE: Adds a folder to base path string
		*/
		void setBasePath(const std::string& path);

		/*
		* IMPORTS: fileName - The name of the file.
		* EXPORTS: std::string - path to file
		* PURPOSE: To retrieve a file
		*/
		std::string retrieve(const std::string& fileName) const;

		/*
		* IMPORTS: none
		* EXPORTS: std::string - base path
		* PURPOSE: To get base path
		*/
		std::string getBasePath() const;

	private:
		std::string m_basePath;
	};
}

#endif
