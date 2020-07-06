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

		nlohmann::json arr = j.at("shaderbook");
		std::for_each(arr.begin(), arr.end(), [this](const nlohmann::json& shader)
		{
			auto vs = FileSystem::s_root + FileSystem::s_shaders + shader[0].get<std::string>();
			auto fs = FileSystem::s_root + FileSystem::s_shaders + shader[1].get<std::string>();

			this->add(vs, fs);
		});
	}

	void ShaderBook::clear() noexcept
	{
		m_resources.clear();
	}
}