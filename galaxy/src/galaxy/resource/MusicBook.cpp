///
/// MusicBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "MusicBook.hpp"

namespace galaxy
{
	namespace res
	{
		MusicBook::MusicBook(std::string_view file)
			: Serializable {this}
		{
			create_from_json(file);
		}

		MusicBook::~MusicBook() noexcept
		{
			clear();
		}

		void MusicBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Musicbook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void MusicBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json MusicBook::serialize()
		{
			nlohmann::json json = "{\"musicbook\":{}}"_json;

			for (const auto& [name, music] : m_resources)
			{
				json["musicbook"][name] = music->serialize();
			}

			return json;
		}

		void MusicBook::deserialize(const nlohmann::json& json)
		{
			clear();

			for (const auto& [name, obj] : json.at("musicbook").items())
			{
				create(name, obj);
			}
		}
	} // namespace res
} // namespace galaxy