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
			ConsoleSink() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~ConsoleSink() noexcept = default;

			///
			/// Sink the message.
			///
			/// \param message String to sink.
			///
			void sink_message(std::string_view message) noexcept override;
		};
	} // namespace error
} // namespace galaxy

#endif