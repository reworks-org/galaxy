///
/// ConsoleSink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_CONSOLESINK_HPP_
#define GALAXY_LOGGING_CONSOLESINK_HPP_

#include "galaxy/logging/Sink.hpp"

namespace galaxy
{
	namespace logging
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
			ConsoleSink();

			///
			/// Destructor.
			///
			virtual ~ConsoleSink();

			///
			/// Sink the message.
			///
			/// \param message Message data to send to sink.
			///
			void sink_message(const LogMessage& message) override;
		};
	} // namespace logging
} // namespace galaxy

#endif
