///
/// Sink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_SINK_HPP_
#define GALAXY_ERROR_SINK_HPP_

#include <string_view>

namespace galaxy
{
	namespace error
	{
		///
		/// A sink to log messages to.
		///
		class Sink
		{
		public:
			///
			/// Constructor.
			///
			Sink() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~Sink() noexcept = default;

			///
			/// Sink the message.
			///
			/// \param message String to sink.
			///
			virtual void sink_message(std::string_view message) noexcept = 0;
		};
	} // namespace error
} // namespace galaxy

#endif