///
/// ShaderBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "ShaderBook.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ShaderBook::ShaderBook(const std::string& json) noexcept
	{
		createFromJSON(json);
	}

	ShaderBook::~ShaderBook() noexcept
	{
		clear();
	}

	void ShaderBook::createFromJSON(const std::string& json)
	{
		std::ifstream is;
		is.open(std::filesystem::path(json).string(), std::ifstream::in);

		nlohmann::json j;
		is >> j;
		is.close();

		std::for_each(j.begin(), j.end(), [this](const nlohmann::json& arr)
		{
			auto pathA = FileSystem::s_root + FileSystem::s_shaders + arr[0].get<std::string>();
			auto pathB = FileSystem::s_root + FileSystem::s_shaders + arr[1].get<std::string>();

			this->add(pathA, pathB);
		});
	}

	void ShaderBook::clear() noexcept
	{
		m_resources.clear();
	}
}