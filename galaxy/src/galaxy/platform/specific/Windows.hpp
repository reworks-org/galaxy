///
/// Windows.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_
#define GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_

#ifdef _WIN32 || _WIN64

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
		/// Pauses current application.
		///
		/// \param path Path to the application.
		///
		void run_process(std::string_view path);

		///
		/// Run an application process within this window.
		///
		/// \param path Path to the application.
		///
		void run_process_within_window(std::string_view path);
	} // namespace platform
} // namespace galaxy

#endif
#endif