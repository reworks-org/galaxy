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
			virtual ~Sink() noexcept = default;

			///
			/// Record the message.
			///
			/// \param message The actual message to record.
			///
			virtual void sink_message(std::string_view message) noexcept = 0;

		protected:
			///
			/// Constructor.
			///
			Sink() noexcept = default;

			///
			/// Copy constructor.
			///
			Sink(const Sink&) noexcept = default;

			///
			/// Move constructor.
			///
			Sink(Sink&&) noexcept = default;

			///
			/// Copy assignment operator.
			///
			Sink& operator=(const Sink&) noexcept = default;

			///
			/// Move assignment operator.
			///
			Sink& operator=(Sink&&) noexcept = default;
		};
	} // namespace error
} // namespace galaxy

#endif