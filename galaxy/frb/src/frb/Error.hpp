///
/// Error.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_ERROR_HPP_
#define FRB_ERROR_HPP_

#include <string>

///
/// Core namespace.
///
namespace frb
{
	///
	/// Processess for an OpenAL error.
	///
	/// \param message Message to print alongside OpenAL error.
	///
	/// \return Move constructed string containing full error message.
	///
	std::string parseError(const std::string& message) noexcept;

	///
	/// Retrieve error code value as string.
	///
	/// \param err Error code from alGetError().
	///
	/// \return Error code enum as a move constructed string.
	///
	std::string errorCodeAsString(const int err) noexcept;
}

#endif