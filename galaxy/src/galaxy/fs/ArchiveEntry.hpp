///
/// ArchiveEntry.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_ARCHIVEENTRY_HPP_
#define GALAXY_FS_ARCHIVEENTRY_HPP_

#include <string>

#include "galaxy/fs/AssetType.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// An entry in an archive in the virtual file system.
		///
		struct ArchiveEntry final
		{
			///
			/// Handle for the zip archive.
			///
			long long index = -1;

			///
			/// Zip archive the entry belongs to.
			///
			std::string pack;

			///
			/// Type of asset.
			///
			AssetType type = AssetType::UNKNOWN;
		};
	} // namespace fs
} // namespace galaxy

#endif
