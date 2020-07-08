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
		auto path = std::filesystem::path(galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + json);

		std::ifstream is(path.string(), std::ifstream::in);
		if (is.good())
		{
			nlohmann::json j;
			is >> j;

			nlohmann::json arr = j.at("shaderbook");
			std::for_each(arr.begin(), arr.end(), [this](const nlohmann::json& shader)
			{
				auto vsPath = std::filesystem::path(FileSystem::s_root + FileSystem::s_shaders + shader[0].get<std::string>());
				auto fsPath = std::filesystem::path(FileSystem::s_root + FileSystem::s_shaders + shader[1].get<std::string>());

				this->add(vsPath.stem().string(), vsPath.string(), fsPath.string());
			});
			
			is.close();
		}
	}

	void ShaderBook::clear() noexcept
	{
		m_resources.clear();
	}
}