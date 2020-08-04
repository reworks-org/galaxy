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
	ShaderBook::ShaderBook(std::string_view json)
	{
		create_from_json(json);
	}

	ShaderBook::~ShaderBook()
	{
		clear();
	}

	void ShaderBook::create_from_json(std::string_view json)
	{
		auto path = std::filesystem::path {fmt::format("{0}{1}{2}", galaxy::FileSystem::s_root, galaxy::FileSystem::s_json, json)};

		std::ifstream ifs(path.string(), std::ifstream::in);
		if (ifs.good())
		{
			nlohmann::json j;
			ifs >> j;

			nlohmann::json arr = j.at("shaderbook");
			std::for_each(arr.begin(), arr.end(), [&](const nlohmann::json& shader) {
				auto vs = std::filesystem::path {
				    fmt::format("{0}{1}{2}", FileSystem::s_root, FileSystem::s_shaders, shader[0].get<std::string>())};
				auto fs = std::filesystem::path {
				    fmt::format("{0}{1}{2}", FileSystem::s_root, FileSystem::s_shaders, shader[1].get<std::string>())};

				create(vs.stem().string(), vs.string(), fs.string());
			});

			ifs.close();
		}
	}

	void ShaderBook::clear()
	{
		m_resources.clear();
	}
} // namespace galaxy