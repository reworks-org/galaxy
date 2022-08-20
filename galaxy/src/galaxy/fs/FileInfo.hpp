///
/// FileInfo.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_FILEINFO_HPP_
#define GALAXY_FS_FILEINFO_HPP_

#include <filesystem>

#include "galaxy/fs/FileCode.hpp"

namespace galaxy
{
	namespace fs
	{
		struct FileInfo final
		{
			///
			/// The error code returned.
			///
			FileCode m_code = FileCode::DEFAULT;

			///
			/// A string formatted absolute path to the file.
			///
			std::string m_string;

			///
			/// A filesystem aboslute path to the file.
			///
			std::filesystem::path m_path;
		};
	} // namespace fs
} // namespace galaxy

#endif