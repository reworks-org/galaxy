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
		void clipboard_set(const std::string& contents) noexcept;

		///
		/// Get clipboard contents.
		///
		/// \return UTF-8 string.
		///
		[[nodiscard]]
		std::string clipboard_get() noexcept;

		///
		/// Clear the clipboard contents.
		///
		/// \return True if successful.
		///
		[[maybe_unused]]
		bool clipboard_clear() noexcept;
	} // namespace input
} // namespace galaxy

#endif
