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
			nlohmann::json json = json::parse_from_disk(file);

			for (const auto& [name, arr] : json.at("soundbook").items())
			{
				create(name, arr);
			}
		}

		void SoundBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy