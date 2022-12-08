///
/// FileSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <filesystem>

#include <zip.h>

#include "FileSink.hpp"

namespace galaxy
{
	namespace error
	{
		FileSink::FileSink(std::string_view file) noexcept
		{
			const auto filepath    = std::filesystem::path(file);
			const auto parent_path = filepath.parent_path();
			const auto zip_path    = parent_path / "old_logs.zip";

			struct zip_t* zip = nullptr;
			if (std::filesystem::exists(zip_path))
			{
				zip = zip_open(zip_path.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'a');
			}
			else
			{
				zip = zip_open(zip_path.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
			}

			for (const auto& path : std::filesystem::recursive_directory_iterator(parent_path))
			{
				const auto entry_path = std::filesystem::path(path);
				if (std::filesystem::is_regular_file(entry_path) && entry_path.extension() == ".log" && entry_path.stem() != filepath.stem())
				{
					zip_entry_open(zip, entry_path.filename().string().c_str());
					zip_entry_fwrite(zip, entry_path.string().c_str());
					zip_entry_close(zip);

					std::filesystem::remove(entry_path);
				}
			}

			zip_close(zip);
			m_file_stream.open(filepath, std::ofstream::out);
		}

		FileSink::~FileSink() noexcept
		{
			m_file_stream.close();
		}

		void FileSink::sink_message(std::string_view message) noexcept
		{
			m_file_stream << message;
		}
	} // namespace error
} // namespace galaxy