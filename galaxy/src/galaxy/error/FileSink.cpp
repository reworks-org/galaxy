///
/// FileSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <filesystem>

#include "FileSink.hpp"

namespace galaxy
{
	namespace error
	{
		FileSink::FileSink(std::string_view file) noexcept
		{
			m_file_stream.open(std::filesystem::path(file).string(), std::ofstream::out);
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