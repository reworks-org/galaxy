///
/// LogStream.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include "LogStream.hpp"

///
/// Core namespace.
///
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

		// Find path
		std::filesystem::path path(logTo);
		std::filesystem::path directory = path.remove_filename();

		// Make sure it exists.
		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
		}

		// Open and make sure its not errored.
		m_fileStream.open(logTo, std::ofstream::out);
		if (m_fileStream.fail())
		{
			throw std::runtime_error("Failed to create log: " + logTo);
		}
	}
}