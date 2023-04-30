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
		/// \brief A source for log messages to be recorded to.
		///
		/// I.e. Console, File, Database...
		///
		class Sink
		{
		public:
			///
			/// Virtual Destructor.
			///
			virtual ~Sink() = default;

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
			virtual void sink_message(std::string_view colour,
				std::string_view level,
				std::string_view time,
				std::string_view file,
				std::string_view line,
				std::string_view message) = 0;

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