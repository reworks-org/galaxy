///
/// PhysFSError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ERROR_PHYSFSERROR_HPP_
#define GALAXY_ERROR_PHYSFSERROR_HPP_

namespace galaxy
{
	namespace error
	{
		///
		/// Call a physfs function with error handling and logs a message for you.
		///
		/// \param code The code returned by physfs function.
		///
		/// \return True if function call succeeded.
		///
		[[maybe_unused]] bool physfs_check(const int code);

		///
		/// Call a physfs function with error handling and logs a message for you.
		///
		/// \param ptr Pointer returned by physfs function.
		///
		/// \return True if function call succeeded.
		///
		[[maybe_unused]] bool physfs_check(void* ptr);
	} // namespace error
} // namespace galaxy

#endif
