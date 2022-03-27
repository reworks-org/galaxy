///
/// Musical.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Musical.hpp"

namespace galaxy
{
	namespace resource
	{
		Musical::Musical() noexcept
		{
		}

		std::shared_ptr<audio::Music> Musical::create(std::string_view file)
		{
			auto sound = std::make_shared<audio::Music>();
			sound->load(file);

			return sound;
		}

		Holder<audio::Music> Musical::create_from_folder(std::string_view folder)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto cache = Holder<audio::Music>();

			for (const auto& file : fs.list_directory(folder))
			{
				const auto key = std::filesystem::path(file).stem().string();

				cache[key] = std::make_shared<audio::Music>();
				cache[key]->load(file);
			}

			return cache;
		}

		nlohmann::json Musical::internal_serialize(Holder<audio::Music>& holder)
		{
			nlohmann::json json = "{}"_json;

			for (const auto& [name, music] : holder)
			{
				json[name] = music->serialize();
			}

			return json;
		}

		Holder<audio::Music> Musical::internal_deserialize(const nlohmann::json& json)
		{
			auto cache = Holder<audio::Music>();

			for (const auto& [name, obj] : json.items())
			{
				cache[name] = std::make_shared<audio::Music>(obj);
			}

			return cache;
		}
	} // namespace resource
} // namespace galaxy