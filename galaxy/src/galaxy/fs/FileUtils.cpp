///
/// FileUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <nlohmann/json.hpp>
#include <SDL3/SDL_misc.h>

#include "galaxy/logging/Log.hpp"

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

			if (buffer.empty())
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

		std::optional<nlohmann::json> read_json(const std::string& filepath)
		{
			const auto read = fileutils::read(filepath);
			if (read)
			{
				return std::make_optional(nlohmann::json::parse(read.value()));
			}
			else
			{
				read.error().log();
				return std::nullopt;
			}
		}

		bool write_json(const std::string& filepath, const nlohmann::json& json)
		{
			const auto write = fileutils::write(filepath, json.dump(4));
			if (write)
			{
				write.value().log();
				return false;
			}

			return true;
		}

		void open_url(const std::string& url) noexcept
		{
			if (!SDL_OpenURL(url.c_str()))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open url: {0}.", SDL_GetError());
			}
		}
	} // namespace fileutils
} // namespace galaxy
