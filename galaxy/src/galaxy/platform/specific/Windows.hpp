///
/// Windows.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_
#define GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_

#if defined(_WIN32) || defined(_WIN64)

#include <string_view>

namespace galaxy
{
	namespace platform
	{
		///
		/// Sets up debug terminal.
		///
		void configure_terminal();

		///
		/// \brief Run an application process.
		///
		/// You must call close_process() on returned void pointer to terminate.
		///
		/// \param path Path to the application.
		///
		/// \return Process handle cast as void pointer.
		///
		void* run_process(std::string_view path);

		///
		/// Close an open process.
		///
		/// \param process Process handle cast to a void pointer.
		///
		void close_process(void* process);
	} // namespace platform
} // namespace galaxy

#endif
#endif