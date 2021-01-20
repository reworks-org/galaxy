///
/// ShaderBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "ShaderBook.hpp"

namespace galaxy
{
	namespace res
	{
		ShaderBook::ShaderBook(std::string_view file)
		{
			create_from_json(file);
		}

		ShaderBook::~ShaderBook() noexcept
		{
			clear();
		}

		void ShaderBook::create_from_json(std::string_view file)
		{
			nlohmann::json json = json::parse_from_disk(file);

			for (auto& [name, arr] : json.at("shaderbook").items())
			{
				create(name, arr[0].get<std::string>(), arr[1].get<std::string>());
			}
		}

		void ShaderBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy