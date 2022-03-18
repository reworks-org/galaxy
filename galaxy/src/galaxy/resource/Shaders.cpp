///
/// Shaders.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <ranges>

#include <nlohmann/json.hpp>

#include "galaxy/utils/StringUtils.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Shaders.hpp"

namespace galaxy
{
	namespace resource
	{
		Shaders::Shaders() noexcept
		{
		}

		Shaders::Shaders(std::string_view folder)
		{
			create_from_folder(folder);
		}

		Shaders::Shaders(const nlohmann::json& json)
		{
			internal_deserialize(json);
		}

		std::shared_ptr<graphics::Shader> Shaders::create(std::string_view file)
		{
			auto file_str = static_cast<std::string>(file);

			return std::make_shared<graphics::Shader>(file_str + GALAXY_VERTEX_EXT, file_str + GALAXY_FRAGMENT_EXT);
		}

		Holder<graphics::Shader> Shaders::create_from_folder(std::string_view folder)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto files = fs.list_directory(folder);

			for (auto& file : files)
			{
				file = strutils::replace_first(file, GALAXY_VERTEX_EXT, "");
				file = strutils::replace_first(file, GALAXY_FRAGMENT_EXT, "");
			}

			auto unique_range = std::ranges::unique(files);
			files.erase(unique_range.begin(), unique_range.end());

			std::ranges::sort(files);
			unique_range = std::ranges::unique(files);
			files.erase(unique_range.begin(), unique_range.end());

			auto cache = Holder<graphics::Shader>();

			for (const auto& file : files)
			{
				cache[std::filesystem::path(file).stem().string()] = std::make_shared<graphics::Shader>(file + GALAXY_VERTEX_EXT, file + GALAXY_FRAGMENT_EXT);
			}

			return cache;
		}

		nlohmann::json Shaders::internal_serialize(Holder<graphics::Shader>& holder)
		{
			nlohmann::json json = "[]"_json;

			for (const auto& [name, shader] : holder)
			{
				json.push_back(name);
			}

			return json;
		}

		Holder<graphics::Shader> Shaders::internal_deserialize(const nlohmann::json& json)
		{
			auto cache = Holder<graphics::Shader>();

			// Have to cast to std::string explicitly here.
			for (const std::string& name : json)
			{
				cache[name] = std::make_shared<graphics::Shader>(name + GALAXY_VERTEX_EXT, name + GALAXY_FRAGMENT_EXT);
			}

			return cache;
		}
	} // namespace resource
} // namespace galaxy