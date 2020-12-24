///
/// JSONUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_JSONUTILS_HPP_
#define GALAXY_JSONUTILS_HPP_

#include <span>
#include <string_view>

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Script identification namespace.
	///
	namespace json
	{
		[[nodiscard]] nlohmann::json parse_from_disk(std::string_view file);
		[[nodiscard]] nlohmann::json parse_from_mem(std::span<char> memory);

		void save_to_disk(std::string_view path, const nlohmann::json& json);
	} //namespace json
} // namespace galaxy

#endif