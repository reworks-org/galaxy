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
		friend struct Log;
	public:
		inline ~LogStream()
		{
			fileStream.close();
		}

		template<typename T>
		inline LogStream& operator<<(const T& input)
		{
			std::cout << input;
			fileStream << input;

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

			fileStream.open(logTo, std::ofstream::out);

			if (fileStream.fail())
			{
				throw std::runtime_error("Failed to create log: " + logTo);
			}
		}

	private:
		std::ofstream fileStream;
	};
}

#endif