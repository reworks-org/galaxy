///
/// ObjectLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "ObjectLayer.hpp"

namespace galaxy
{
	namespace map
	{
		ObjectLayer::ObjectLayer()
		{
			GALAXY_LOG(GALAXY_FATAL, "Cannot instantiate a default constructed ObjectLayer.");
		}

		ObjectLayer::ObjectLayer(const nlohmann::json& json)
		    : Layer {json}, m_draw_order {""}
		{
			if (json.count("draworder") > 0)
			{
				m_draw_order = json.at("draworder");
			}

			if (json.count("objects") > 0)
			{
				const auto& object_array = json.at("objects");
				for (const auto& object : object_array)
				{
					m_objects.emplace_back(object);
				}
			}
		}

		ObjectLayer::~ObjectLayer() noexcept
		{
			m_objects.clear();
		}

		const std::string& ObjectLayer::get_compression() const noexcept
		{
			return m_draw_order;
		}

		const std::vector<Object>& ObjectLayer::get_objects() const noexcept
		{
			return m_objects;
		}
	} // namespace map
} // namespace galaxy