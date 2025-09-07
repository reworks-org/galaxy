///
/// Clipboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CLIPBOARD_HPP_
#define GALAXY_INPUT_CLIPBOARD_HPP_

#include <string>

namespace galaxy
{
	namespace input
	{
		///
		/// Set clipboard contents.
		///
		/// \param contents UTF-8 string.
		///
		void set_clipboard(const std::string& contents) noexcept;

		///
		/// Get clipboard contents.
		///
		/// \return UTF-8 string.
		///
		[[nodiscard]]
		std::string get_clipboard() noexcept;
	} // namespace input
} // namespace galaxy

#endif
