///
/// ALError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ERROR_ALERROR_HPP_
#define GALAXY_ERROR_ALERROR_HPP_

#include <string_view>

namespace galaxy
{
	namespace error
	{
		///
		/// Processess for an OpenAL error.
		///
		/// \param message Message to print alongside OpenAL error.
		/// \param error OpenAL error value.
		///
		/// \return String containing full error message.
		///
		[[nodiscard]] std::string al_parse_error(std::string_view message, const int error);

		///
		/// Retrieve error code value as string.
		///
		/// \param err Error code from alGetError().
		///
		/// \return Error code enum as a string.
		///
		[[nodiscard]] std::string al_errcode_as_string(const int err) noexcept;
	} // namespace error
} // namespace galaxy
#endif