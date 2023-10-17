///
/// FileCode.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_FILECODE_HPP_
#define GALAXY_FS_FILECODE_HPP_

namespace galaxy
{
	namespace fs
	{
		///
		/// Used to determine type of filesystem error.
		///
		enum class FileCode : int
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
			NO_EXTENSION,

			///
			/// Used for default initialization.
			///
			DEFAULT
		};
	} // namespace fs
} // namespace galaxy

#endif
