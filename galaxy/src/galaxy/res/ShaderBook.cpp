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

			const std::string vs_ext = json.at("vertex-extension");
			const std::string fs_ext = json.at("fragment-extension");
			const auto& arr          = json.at("shaderbook");

			for (const std::string& filename : arr)
			{
				create(filename, filename + vs_ext, filename + fs_ext);
			}
		}

		void ShaderBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy