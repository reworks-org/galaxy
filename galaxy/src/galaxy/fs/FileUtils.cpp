///
/// FileUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "FileUtils.hpp"

namespace galaxy
{
	namespace fileutils
	{
		std::optional<std::string> extension(const std::string& filepath) noexcept
		{
			const auto path = std::filesystem::path(filepath);
			if (path.has_extension())
			{
				return std::make_optional(path.extension().string());
			}

			return std::nullopt;
		}

		std::expected<std::string, FileError> read(const std::string& filepath)
		{
			const auto path = std::filesystem::path(filepath);
			if (!std::filesystem::exists(path))
			{
				return std::unexpected(FileError("read", "File does not exist.", path));
			}

			if (!std::filesystem::is_regular_file(path))
			{
				return std::unexpected(FileError("read", "Path is an irregular file or a directory", path));
			}

			std::ifstream input {path, std::ifstream::in};
			if (!input.good())
			{
				return std::unexpected(FileError("read", "Permission denied.", path));
			}

			std::string buffer(std::filesystem::file_size(path), '\0');
			input.read(buffer.data(), buffer.size());
			if (!input.good())
			{
				return std::unexpected(FileError("read", "Failed to read file.", path));
			}

			return buffer;
		}

		std::optional<FileError> write(const std::string& filepath, const std::string& data)
		{
			const auto path = std::filesystem::path(filepath);
			if (std::filesystem::exists(path))
			{
				if (std::filesystem::is_directory(path))
				{
					return std::make_optional(FileError("write", "Tried to write to a directory not a file.", path));
				}
			}

			std::ofstream ofs {path, std::ofstream::out | std::ofstream::trunc};
			if (!ofs.good())
			{
				return std::make_optional(FileError("write", "Permission denied.", path));
			}

			ofs << data;

			if (ofs.fail())
			{
				return std::make_optional(FileError("write", "Failed to write into file.", path));
			}

			return std::nullopt;
		}
	} // namespace fileutils
} // namespace galaxy
