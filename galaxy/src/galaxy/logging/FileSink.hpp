///
/// FileSink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_FILESINK_HPP_
#define GALAXY_LOGGING_FILESINK_HPP_

#include <fstream>

#include "galaxy/logging/Sink.hpp"

namespace galaxy
{
	class FileSink final : public Sink
	{
	public:
		///
		/// Constructor.
		///
		/// \param file File to write all log messages to.
		///
		FileSink(std::string_view file);

		///
		/// Destructor.
		///
		virtual ~FileSink() noexcept;

		///
		/// Sink the message.
		///
		/// \param message Message data to send to sink.
		///
		void sink(const LogMessage& message) override;

	private:
		///
		/// File stream to write to.
		///
		std::ofstream m_file_stream;
	};
} // namespace galaxy

#endif
