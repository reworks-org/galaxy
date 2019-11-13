///
/// LogStream.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOGSTREAM_HPP_
#define PULSAR_LOGSTREAM_HPP_

#include <mutex>
#include <future>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace pl
{
	///
	/// Custom log stream enabling filtering, multithreading and stream macros.
	///
	class LogStream
	{
		friend class Log;
	public:
		///
		/// Constructor.
		///
		/// \param disabled Is the stream enabled.
		///
		LogStream(bool disabled);

		///
		/// Destructor.
		///
		~LogStream();

		///
		/// Overloading the output stream operator to gain more control.
		///
		/// \param input Input from stream.A
		///
		/// \return Returns stream operator.
		///
		template<typename T>
		LogStream& operator<<(const T& input);

	private:
		///
		/// Internal function to initialize log stream.
		///
		/// \param logTo File to log output to.
		///
		void init(const std::string& logTo);

	private:
		///
		/// Mutex to protect from multiple accesses.
		///
		std::mutex m_lock;

		///
		/// Disabled flag.
		///
		bool m_disabled = false;

		///
		/// File stream to write to.
		///
		std::ofstream m_fileStream;
	};

	template<typename T>
	inline LogStream& LogStream::operator<<(const T& input)
	{
		// Lock guard prevents dangling threads and access to the same memory by two different threads.
		std::lock_guard<std::mutex> lock(m_lock);
		
		// Launch thread to log to.
		auto result = std::async(std::launch::async, [&]()
		{
			if (!m_disabled)
			{
				std::cout << input;
				m_fileStream << input;
			}
		});

		return *this;
	}
}

#endif