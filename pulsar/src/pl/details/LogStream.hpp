///
/// LogStream.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOGSTREAM_HPP_
#define PULSAR_LOGSTREAM_HPP_

#include <mutex>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace pl
{
	class LogStream
	{
		friend class Log;
	public:
		LogStream(bool disabled);

		~LogStream();

		template<typename T>
		LogStream& operator<<(const T& input);

	private:
		void init(const std::string& logTo);

	private:
		std::mutex m_lock;
		bool m_disabled = false;
		std::ofstream m_fileStream;
	};

	template<typename T>
	inline LogStream& LogStream::operator<<(const T& input)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		if (!m_disabled)
		{
			std::cout << input;
			m_fileStream << input;
		}

		return *this;
	}
}

#endif