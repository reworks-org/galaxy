///
/// VirtualFS.hpp
/// starlight
///
/// Created by reworks on 12/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VIRTUALFS_HPP_
#define STARLIGHT_VIRTUALFS_HPP_

#include <string>

namespace sl
{
	class VirtualFS
	{
	public:
		///
		/// Constructor.
		///
		VirtualFS();
		
		///
		/// Constructor, also takes a parameter to call mount().
		///
		/// \param archive Path or archive to mount.
		///
		VirtualFS(const std::string& archive);

		///
		/// Destructor.
		//
		~VirtualFS();

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
		/// \param file The file in the VirtualFS to open.
		///
		/// \return Returns a std::string. This is a plain string containing the information.
		///
		std::string openAsString(const std::string& file);
	};
}

#endif