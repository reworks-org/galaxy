///
/// ConsoleSink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_CONSOLESINK_HPP_
#define GALAXY_ERROR_CONSOLESINK_HPP_

#include "galaxy/error/Sink.hpp"

namespace galaxy
{
	namespace error
	{
		///
		/// Logs a message to the console.
		///
		class ConsoleSink final : public Sink
		{
		  public:
			///
			/// Constructor.
			///
			ConsoleSink() = default;

			///
			/// Destructor.
			///
			virtual ~ConsoleSink() = default;

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
				std::string_view               level,
				std::string_view               time,
				std::string_view               file,
				std::string_view               line,
				std::string_view               message) override;
		};
	} // namespace error
} // namespace galaxy

#endif
