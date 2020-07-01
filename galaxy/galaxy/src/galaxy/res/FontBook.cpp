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
		std::ifstream is;
		is.open(std::filesystem::path(json).string(), std::ifstream::in);

		nlohmann::json j;
		is >> j;
		is.close();

		std::for_each(j.begin(), j.end(), [this](const nlohmann::json& arr)
		{
			auto path = FileSystem::s_root + FileSystem::s_fonts + arr[0].get<std::string>();
			this->add(path, arr[1].get<int>());
		});
	}

	void FontBook::clear() noexcept
	{
		m_resources.clear();
	}
}