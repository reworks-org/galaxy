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
		FileSink::FileSink(std::string_view file)
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

		FileSink::~FileSink()
		{
			m_file_stream.close();
		}

		void FileSink::sink_message(std::string_view colour,
			std::string_view                         level,
			std::string_view                         time,
			std::string_view                         file,
			std::string_view                         line,
			std::string_view                         message)
		{
			auto level_spaced = static_cast<std::string>(level);
			level_spaced.push_back(':');
			while (level_spaced.length() < 9)
			{
				level_spaced.push_back(' ');
			}

			m_file_stream << std::format("[{0}] {1}[{2}] {3}\n", time, level_spaced, std::format("{0}, Ln {1}", file, line), message);
		}
	} // namespace error
} // namespace galaxy
