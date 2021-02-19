///
/// SoundBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "SoundBook.hpp"

namespace galaxy
{
	namespace res
	{
		SoundBook::SoundBook(std::string_view file)
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
				auto& json = json_opt.value();
				for (const auto& [name, obj] : json.at("soundbook").items())
				{
					create(name, obj);
				}
			}
		}

		void SoundBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy