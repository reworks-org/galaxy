///
/// LogStream.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include "LogStream.hpp"

namespace pl
{
	LogStream::LogStream(bool disabled)
		:m_disabled(disabled)
	{
	}

	LogStream::~LogStream()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_fileStream.close();
	}

	void LogStream::init(const std::string& logTo)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		std::filesystem::path path(logTo);
		std::filesystem::path directory = path.remove_filename();

		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
		}

		m_fileStream.open(logTo, std::ofstream::out);

		if (m_fileStream.fail())
		{
			throw std::runtime_error("Failed to create log: " + logTo);
		}
	}
}