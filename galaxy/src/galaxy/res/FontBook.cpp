///
/// FontBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "FontBook.hpp"

namespace galaxy
{
	namespace res
	{
		FontBook::FontBook(std::string_view json)
		{
			create_from_json(json);
		}

		FontBook::~FontBook()
		{
			clear();
		}

		void FontBook::create_from_json(std::string_view json)
		{
			auto path        = fmt::format("{0}{1}{2}", fs::s_root, fs::s_json, json);
			nlohmann::json j = galaxy::json::parse_from_disk(path);

			nlohmann::json arr = j.at("fontbook");
			std::for_each(arr.begin(), arr.end(), [&](const nlohmann::json& font) {
				auto fp        = std::filesystem::path {fmt::format("{0}{1}{2}", fs::s_root, fs::s_fonts, font[0].get<std::string>())};
				const int size = font[1].get<int>();

				create(fp.stem().string() + std::to_string(size), fp.string(), size);
			});
		}

		void FontBook::clear()
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy