///
/// FileSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "galaxy/utils/Globals.hpp"

#include "FileSink.hpp"

namespace galaxy
{
	namespace error
	{
		bool is_older_than(const std::filesystem::path& path, int hrs)
		{
			return std::chrono::duration_cast<std::chrono::hours>(std::filesystem::file_time_type::clock::now() - std::filesystem::last_write_time(path))
					   .count() > hrs;
		}

		FileSink::FileSink(std::string_view file) noexcept
		{
			const auto filepath    = std::filesystem::path(file);
			const auto parent_path = filepath.parent_path();

			for (const auto& path : std::filesystem::recursive_directory_iterator(parent_path))
			{
				if (std::filesystem::is_regular_file(path) && is_older_than(path, GALAXY_REMOVE_LOG_FILES_OLDER_THAN_HOURS))
				{
					std::filesystem::remove(path);
				}
			}

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