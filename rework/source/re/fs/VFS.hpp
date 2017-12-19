///
///  VFS.hpp
///  rework
///
///  Created by reworks on 12/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_VFS_HPP_
#define REWORK_VFS_HPP_

#include <string>

#include "re/types/ServiceLocator.hpp"

namespace re
{
	class VFS : public ServiceLocator<VFS>
	{
	public:
		///
		/// Constructor.
		///
		VFS();
		
		///
		/// Constructor, also takes a parameter to call mount().
		///
		/// \param archive Path or archive to mount.
		///
		VFS(const std::string& archive);

		///
		/// Destructor.
		//
		~VFS() override;

		///
		/// \brief Mount an archive or folder.
		///
		/// This is done by physfs so it supports the most common archive types. You can also point to a folder.
		///
		/// \param archive Path or archive to mount.
		///
		void mount(const std::string& archive);

		///
		/// \brief Open a file and return as a std::string.
		///
		/// Please note this string is self-contained. You don't need to free anything. Also note this only supports files that contain text!
		///
		/// \param file The file in the VFS to open.
		///
		/// \return Returns a std::string. This is a plain string containing the information.
		///
		std::string openAsString(const std::string& file);

	private:
		///
		/// Copy Constructor.
		/// Deleted.
		///
		VFS(const VFS&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		VFS(VFS&&) = delete;
	};
}

#endif