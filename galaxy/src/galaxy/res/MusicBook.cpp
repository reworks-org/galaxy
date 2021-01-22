///
/// MusicBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "MusicBook.hpp"

namespace galaxy
{
	namespace res
	{
		MusicBook::MusicBook(std::string_view file)
		{
			create_from_json(file);
		}

		MusicBook::~MusicBook() noexcept
		{
			clear();
		}

		void MusicBook::create_from_json(std::string_view file)
		{
			nlohmann::json json = json::parse_from_disk(file);

			for (auto& [name, arr] : json.at("musicbook").items())
			{
				create(name, arr);
			}
		}

		void MusicBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy