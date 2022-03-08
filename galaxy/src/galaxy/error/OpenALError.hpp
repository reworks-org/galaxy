///
/// OpenALError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ERROR_OPENALERROR_HPP_
#define GALAXY_ERROR_OPENALERROR_HPP_

#include <string_view>

struct ALCdevice;

namespace galaxy
{
	namespace error
	{
		///
		/// Processess for an OpenAL error.
		///
		/// \param message Message to print alongside OpenAL error.
		/// \param err Error code from alGetError().
		///
		/// \return String containing full error message.
		///
		[[nodiscard]] std::string al_handle_error(std::string_view message, const int err);

		///
		/// Retrieve error code value as string.
		///
		/// \param err Error code from alGetError().
		///
		/// \return Error code enum as a string.
		///
		[[nodiscard]] std::string al_errcode_as_string(const int err) noexcept;

		///
		/// Processess for an OpenAL context error.
		///
		/// \param device Context device error occured in.
		/// \param message Message to print alongside OpenAL context error.
		/// \param err Error code from alcGetError().
		///
		/// \return String containing full error message.
		///
		[[nodiscard]] std::string alc_handle_error(ALCdevice* device, std::string_view message, const int err);

		///
		/// Retrieve error code value as string.
		///
		/// \param err Error code from alcGetError().
		///
		/// \return Error code enum as a string.
		///
		[[nodiscard]] std::string alc_errcode_as_string(const int err) noexcept;
	} // namespace error
} // namespace galaxy

#endif