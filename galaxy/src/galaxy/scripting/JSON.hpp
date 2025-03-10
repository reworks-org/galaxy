///
/// JSON.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_JSON_HPP_
#define GALAXY_SCRIPTING_JSON_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace json
	{
		///
		/// Read a json file in the vfs.
		///
		/// \param file File in vfs to read and parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]]
		nlohmann::json read(const std::string& file);

		///
		/// Read raw json string.
		///
		/// \param json Raw json string to parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]]
		nlohmann::json read_raw(const std::string& json);

		///
		/// Write json to vfs.
		///
		/// \param file File in the vfs to write to.
		/// \param json Json data to write.
		///
		/// \return True if file successfully saved.
		///
		[[nodiscard]]
		bool write(const std::string& file, const nlohmann::json& json);

		///
		/// Dump json to string.
		///
		/// \param json Json data to dump.
		///
		/// \return Json in string format.
		///
		[[nodiscard]]
		std::string dump(const nlohmann::json& json);
	} // namespace json
} // namespace galaxy

#endif
