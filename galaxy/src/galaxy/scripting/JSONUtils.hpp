///
/// JSONUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_JSONUTILS_HPP_
#define GALAXY_SCRIPTING_JSONUTILS_HPP_

#include <optional>
#include <span>
#include <string_view>

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace json
	{
		///
		/// Load and parse a json file from disk.
		///
		/// \param file File to read and parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]] std::optional<nlohmann::json> parse_from_disk(std::string_view file);

		///
		/// Load and parse a json file from memory.
		///
		/// \param memory Memory to read and parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]] std::optional<nlohmann::json> parse_from_mem(std::span<char> memory);

		///
		/// Save a json file to disk.
		///
		/// \param path Path to the file to save.
		/// \param json Json to write to file.
		///
		/// \return Const bool. True if file successfully saved to disk.
		///
		[[nodiscard]] const bool save_to_disk(std::string_view path, const nlohmann::json& json);
	} //namespace json
} // namespace galaxy

#endif