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
	FontBook::FontBook(const std::string& json) noexcept
	{
		createFromJSON(json);
	}

	FontBook::~FontBook() noexcept
	{
		clear();
	}

	void FontBook::createFromJSON(const std::string& json)
	{
		auto path = std::filesystem::path(galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + json);

		std::ifstream is(path.string(), std::ifstream::in);
		if (is.good())
		{
			nlohmann::json j;
			is >> j;

			nlohmann::json arr = j.at("fontbook");
			std::for_each(arr.begin(), arr.end(), [this](const nlohmann::json& font)
			{
				auto fontPath = std::filesystem::path(FileSystem::s_root + FileSystem::s_fonts + font[0].get<std::string>());
				this->add(fontPath.stem().string(), fontPath.string(), font[1].get<int>());
			});
			
			is.close();
		}
	}

	void FontBook::clear() noexcept
	{
		m_resources.clear();
	}
}