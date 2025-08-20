///
/// FileUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_FILEUTILS_HPP_
#define GALAXY_FS_FILEUTILS_HPP_

#include <expected>
#include <optional>

#include "galaxy/fs/FileError.hpp"

namespace galaxy
{
	namespace fileutils
	{
		///
		/// Get a file or path's extension.
		///
		/// \param filepath Filename or path with a filename.
		///
		/// \return Extension, including dot. Nullopt if errors.
		///
		[[nodiscard]]
		std::optional<std::string> extension(const std::string& filepath) noexcept;

		///
		/// Read a non-binary file on disk.
		///
		/// \param filepath Path to file to read.
		///
		/// \return Data, or filerror if an unexpected error occured.
		///
		[[nodiscard]]
		std::expected<std::string, FileError> read(const std::string& filepath);

		///
		/// Writes a non-binary file to disk.
		///
		/// \param filepath Path to file to write to.
		/// \param data Data to write to file.
		///
		/// \return Nullopt on success, FileError on failure.
		///
		[[nodiscard]]
		std::optional<FileError> write(const std::string& filepath, const std::string& data);
	} // namespace fileutils
} // namespace galaxy

#endif
