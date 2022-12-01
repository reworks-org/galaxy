///
/// StringUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_STRINGUTILS_HPP_
#define GALAXY_UTILS_STRINGUTILS_HPP_

#include <string_view>
#include <vector>

namespace galaxy
{
	namespace strutils
	{
		///
		/// Split a string based on a delimiter.
		///
		/// \param input Input string to process.
		/// \param delim Delimiter to split string on.
		///
		/// \return Array of strings split. Does not return empty entries or delimiter.
		///
		[[nodiscard]] std::vector<std::string> split(std::string_view input, std::string_view delim) noexcept;

		///
		/// \brief Replace first occurance of a string.
		///
		/// Returns input string if to_replace not found.
		///
		/// \param input String to modify.
		/// \param to_replace Substring to be replaced.
		/// \param replace_with String to be inserted.
		///
		/// \return Modified string.
		///
		[[nodiscard]] std::string& replace_first(std::string& input, std::string_view to_replace, std::string_view replace_with) noexcept;

		///
		/// \brief Replaces all occurances of a string.
		///
		/// Returns input string if to_replace not found.
		///
		/// \param input String to modify.
		/// \param to_replace Substring to be replaced.
		/// \param replace_with String to be inserted.
		///
		/// \return Modified string.
		///
		[[nodiscard]] std::string& replace_all(std::string& input, std::string_view to_replace, std::string_view replace_with) noexcept;

		///
		/// Check if string begins with another string.
		///
		/// \param input Input string to check.
		/// \param find String to check for at beginning of input string.
		///
		/// \return True if input string begins with find string.
		///
		[[nodiscard]] bool begins_with(const std::string& input, const std::string& find) noexcept;
	} // namespace strutils
} // namespace galaxy

#endif