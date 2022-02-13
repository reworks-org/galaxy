///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializer.hpp"
#include "galaxy/scripting/JSON.hpp"

namespace galaxy
{
	namespace fs
	{
		Serializer::Serializer()
		{
			// Reserve some initial memory.
			m_data.reserve(100);
		}

		Serializer::~Serializer() noexcept
		{
			clear();
		}

		void Serializer::add(Serializable* obj) noexcept
		{
			m_data.push_back(obj);
		}

		void Serializer::clear() noexcept
		{
			m_data.clear();
		}

		std::future<void> Serializer::save_all(std::string_view file)
		{
			// Offload work to another thread.
			return std::async(std::launch::async, [this, file]() {
				auto data = nlohmann::json::object();

				for (Serializable* obj : m_data)
				{
					data[obj->get_key()] = obj->serialize();
				}

				if (!json::save_to_disk(file, data))
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to serialize json to disk for file: {0}.", file);
				}
			});
		}

		std::future<void> Serializer::load_all(std::string_view file)
		{
			// Offload work to another thread.
			return std::async(std::launch::async, [this, file]() {
				auto data_opt = json::parse_from_disk(file);

				if (data_opt.has_value())
				{
					auto& data = data_opt.value();

					for (Serializable* obj : m_data)
					{
						if (data.contains(obj->get_key()))
						{
							obj->deserialize(data[obj->get_key()]);
						}
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to deserialize file: {0}.", file);
				}
			});
		}
	} // namespace fs
} // namespace galaxy