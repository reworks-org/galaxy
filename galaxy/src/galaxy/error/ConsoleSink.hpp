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
	} // namespace error
} // namespace galaxy

#endif
