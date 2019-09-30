///
/// LogStream.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOGSTREAM_HPP_
#define PULSAR_LOGSTREAM_HPP_

#include <fstream>
#include <iostream>
#include <filesystem>

namespace pl
{
	class LogStream
	{
		friend class Log;
	public:
		LogStream(bool disabled)
		{
			m_disabled = disabled;
		}

		inline ~LogStream()
		{
			m_fileStream.close();
		}

		template<typename T>
		inline LogStream& operator<<(const T& input)
		{
			if (!m_disabled)
			{
				std::cout << input;
				m_fileStream << input;
			}

			return *this;
		}

	private:
		inline void init(const std::string& logTo)
		{
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

	private:
		bool m_disabled = false;
		std::ofstream m_fileStream;
	};
}

#endif