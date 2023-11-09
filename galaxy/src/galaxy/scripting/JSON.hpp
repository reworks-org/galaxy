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
		/// \param entry Entry in vfs to read and parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]] std::optional<nlohmann::json> read_vfs(const std::string& entry);

		///
		/// Read raw json string.
		///
		/// \param json Raw json string to parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]] std::optional<nlohmann::json> read_raw(const std::string& json);

		///
		/// Load and parse a json file from disk.
		///
		/// \param file File to read and parse.
		///
		/// \return Fully parsed json object.
		///
		[[nodiscard]] std::optional<nlohmann::json> read_disk(const std::string& file);

		///
		/// Write json to vfs.
		///
		/// \param Entry Entry in the vfs to write to.
		/// \param json Json data to write.
		///
		/// \return True if file successfully saved.
		///
		[[nodiscard]] bool write_vfs(const std::string& entry, const nlohmann::json& json);

		///
		/// Save a json file to disk.
		///
		/// \param file Path to the file to save.
		/// \param json Json to write to file.
		///
		/// \return True if file successfully saved to disk.
		///
		[[nodiscard]] bool write_disk(const std::string& file, const nlohmann::json& json);
	} // namespace json
} // namespace galaxy

#endif
