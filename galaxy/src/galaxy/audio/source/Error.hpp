///
/// Error.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_ERROR_HPP_
#define FRB_ERROR_HPP_

#include <string_view>

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
	/// \return String containing full error message.
	///
	[[maybe_unused]] std::string parse_error(std::string_view message);

	///
	/// Retrieve error code value as string.
	///
	/// \param err Error code from alGetError().
	///
	/// \return Error code enum as a string.
	///
	[[nodiscard]] std::string errcode_as_string(const int err);
} // namespace frb

#endif