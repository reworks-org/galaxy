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
	///
	/// Logs a message to the console.
	///
	class ConsoleSink final : public Sink
	{
	public:
		///
		/// Constructor.
		///
		ConsoleSink() noexcept;

		///
		/// Destructor.
		///
		virtual ~ConsoleSink() noexcept;

		///
		/// Sink the message.
		///
		/// \param message Message data to send to sink.
		///
		void sink(const LogMessage& message) override;
	};
} // namespace galaxy

#endif
