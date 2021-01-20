///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Serializer.hpp"

namespace galaxy
{
	namespace fs
	{
		void serialize(std::string_view file)
		{
			nlohmann::json json = "{}"_json;
			json["entities"]    = nlohmann::json::object();

			//world->each([&](const ecs::Entity entity) {
			//json["enabled"] = world->is_enabled(entity);
			//auto* [comps] = world->get_multi<COMPS>(entity);
			// Then call each comp and cast to a serializable pointer.
			// then save.
			//	});

			json::save_to_disk(file, json);
		}

		void deserialize(std::string_view file)
		{
			nlohmann::json json = json::parse_from_disk(file);
			if (json.count("entities") > 0)
			{
				auto entities = json.at("entities");
				for (auto& [entity, data] : entities.items())
				{
					// https://youtu.be/IEiOP7Y-Mbc?t=1705
				}
			}
		}
	} // namespace fs
} // namespace galaxy