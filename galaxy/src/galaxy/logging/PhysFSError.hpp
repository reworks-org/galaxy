///
/// PhysFSError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LOGGING_PHYSFSERROR_HPP_
#define GALAXY_LOGGING_PHYSFSERROR_HPP_

namespace galaxy
{
	namespace logging
	{
		///
		/// Call a physfs function with error handling and logs a message for you.
		///
		/// \param code The code returned by physfs function.
		///
		/// \return True if function call succeeded.
		///
		[[maybe_unused]]
		bool physfs_check(const int code) noexcept;

		///
		/// Call a physfs function with error handling and logs a message for you.
		///
		/// \param ptr Pointer returned by physfs function.
		///
		/// \return True if function call succeeded.
		///
		[[maybe_unused]]
		bool physfs_check(void* ptr) noexcept;
	} // namespace logging
} // namespace galaxy

#endif
