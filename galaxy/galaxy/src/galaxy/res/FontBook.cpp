///
/// FontBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "FontBook.hpp"

///
/// Core namespace.
///
namespace galaxy
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
		auto path = std::filesystem::path {fmt::format("{0}{1}{2}", galaxy::FileSystem::s_root, galaxy::FileSystem::s_json, json)};

		std::ifstream ifs(path.string(), std::ifstream::in);
		if (ifs.good())
		{
			nlohmann::json j;
			ifs >> j;

			nlohmann::json arr = j.at("fontbook");
			std::for_each(arr.begin(), arr.end(), [&](const nlohmann::json& font) {
				auto fp        = std::filesystem::path {fmt::format("{0}{1}{2}", galaxy::FileSystem::s_root, galaxy::FileSystem::s_fonts, font[0].get<std::string>())};
				const int size = font[1].get<int>();

				create(fp.stem().string() + std::to_string(size), fp.string(), size);
			});

			ifs.close();
		}
	}

	void FontBook::clear()
	{
		m_resources.clear();
	}
} // namespace galaxy