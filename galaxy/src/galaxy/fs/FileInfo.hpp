///
/// FileInfo.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_FILEINFO_HPP_
#define GALAXY_FS_FILEINFO_HPP_

#include <filesystem>

namespace galaxy
{
	namespace fs
	{
		struct FileInfo final
		{
			///
			/// Used to determine type of filesystem error.
			///
			enum class Code : int
			{
				///
				/// File was found in VFS.
				///
				FOUND,

				///
				/// File not found.
				///
				NOT_FOUND,

				///
				/// Absolute path is not in the filesystem.
				///
				NOT_IN_VFS,

				///
				/// Is not a file.
				///
				NO_EXTENSION
			};

			///
			/// The error code returned.
			///
			Code m_code;

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