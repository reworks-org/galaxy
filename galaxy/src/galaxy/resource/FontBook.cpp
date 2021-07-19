///
/// FontBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "FontBook.hpp"

namespace galaxy
{
	namespace res
	{
		FontBook::FontBook(std::string_view file)
			: Serializable {this}
		{
			create_from_json(file);
		}

		FontBook::~FontBook() noexcept
		{
			clear();
		}

		void FontBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Fontbook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void FontBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json FontBook::serialize()
		{
			nlohmann::json json = "{\"fontbook\":{}}"_json;

			for (const auto& [name, font] : m_resources)
			{
				json["fontbook"][name]["file"] = font->get_filename();
				json["fontbook"][name]["size"] = font->get_pixel_size();
			}

			return json;
		}

		void FontBook::deserialize(const nlohmann::json& json)
		{
			clear();

			for (auto& [name, obj] : json.at("fontbook").items())
			{
				create(name, obj.at("file"), obj.at("size"));
			}
		}
	} // namespace res
} // namespace galaxy