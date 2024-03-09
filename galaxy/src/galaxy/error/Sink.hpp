///
/// Sink.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_SINK_HPP_
#define GALAXY_ERROR_SINK_HPP_

#include "galaxy/error/LogMessage.hpp"

namespace galaxy
{
	namespace error
	{
		///
		/// \brief A source for log messages to be recorded to.
		///
		/// I.e. Console, File, Database...
		///
		class Sink
		{
		  public:
			///
			/// Virtual destructor.
			///
			virtual ~Sink();

			///
			/// Sink the message.
			///
			/// \param message Message data to send to sink.
			///
			virtual void sink_message(const LogMessage& message) = 0;

		  protected:
			///
			/// Constructor.
			///
			Sink() = default;

			///
			/// Copy constructor.
			///
			Sink(const Sink&) = default;

			///
			/// Move constructor.
			///
			Sink(Sink&&) = default;

			///
			/// Copy assignment operator.
			///
			Sink& operator=(const Sink&) = default;

			///
			/// Move assignment operator.
			///
			Sink& operator=(Sink&&) = default;
		};
	} // namespace error
} // namespace galaxy

#endif
