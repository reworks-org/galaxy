///
/// FileSink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_FILESINK_HPP_
#define GALAXY_ERROR_FILESINK_HPP_

#include <fstream>

#include "galaxy/error/Sink.hpp"

namespace galaxy
{
	namespace error
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
			virtual ~FileSink();

			///
			/// Sink the message.
			///
			/// \param colour ASCII colour code.
			/// \param level Level of message.
			/// \param time Timestamp of message.
			/// \param file File message occured in.
			/// \param line Line the message occured on.
			/// \param message The actual message to record.
			///
			void sink_message(std::string_view colour,
				std::string_view level,
				std::string_view time,
				std::string_view file,
				std::string_view line,
				std::string_view message) override;

		private:
			///
			/// File stream to write to.
			///
			std::ofstream m_file_stream;
		};
	} // namespace error
} // namespace galaxy

#endif