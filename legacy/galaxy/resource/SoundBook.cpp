///
/// SoundBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "SoundBook.hpp"

namespace galaxy
{
	namespace res
	{
		SoundBook::SoundBook(std::string_view file)
			: Serializable {this}
		{
			create_from_json(file);
		}

		SoundBook::~SoundBook() noexcept
		{
			clear();
		}

		void SoundBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Soundbook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void SoundBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json SoundBook::serialize()
		{
			nlohmann::json json = "{\"soundbook\":{}}"_json;

			for (const auto& [name, sound] : m_resources)
			{
				json["soundbook"][name] = sound->serialize();
			}

			return json;
		}

		void SoundBook::deserialize(const nlohmann::json& json)
		{
			clear();

			for (const auto& [name, obj] : json.at("soundbook").items())
			{
				create(name, obj);
			}
		}
	} // namespace res
} // namespace galaxy