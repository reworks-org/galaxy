///
/// Sounds.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Sounds.hpp"

namespace galaxy
{
	namespace resource
	{
		Sounds::Sounds() noexcept
		{
		}

		Sounds::Sounds(std::string_view folder)
		{
			create_from_folder(folder);
		}

		Sounds::Sounds(const nlohmann::json& json)
		{
			internal_deserialize(json);
		}

		std::shared_ptr<audio::Sound> Sounds::create(std::string_view file)
		{
			auto sound = std::make_shared<audio::Sound>();
			sound->load(file);

			return sound;
		}

		Holder<audio::Sound> Sounds::create_from_folder(std::string_view folder)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto cache = Holder<audio::Sound>();

			for (const auto& file : fs.list_directory(folder))
			{
				const auto key = std::filesystem::path(file).stem().string();

				cache[key] = std::make_shared<audio::Sound>();
				cache[key]->load(file);
			}

			return cache;
		}

		nlohmann::json Sounds::internal_serialize(Holder<audio::Sound>& holder)
		{
			nlohmann::json json = "{}"_json;

			for (const auto& [name, sound] : holder)
			{
				json[name] = sound->serialize();
			}

			return json;
		}

		Holder<audio::Sound> Sounds::internal_deserialize(const nlohmann::json& json)
		{
			auto cache = Holder<audio::Sound>();

			for (const auto& [name, obj] : json.items())
			{
				cache[name] = std::make_shared<audio::Sound>(obj);
			}

			return cache;
		}
	} // namespace resource
} // namespace galaxy