///
/// ObjectLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "ObjectLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	ObjectLayer::ObjectLayer()
	{
		PL_LOG(PL_FATAL, "Cannot instantiate a default constructed ObjectLayer.");
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
			auto object_array = json.at("objects");
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

	std::string ObjectLayer::get_compression() const noexcept
	{
		return m_draw_order;
	}

	const auto& ObjectLayer::get_objects() const noexcept
	{
		return m_objects;
	}
} // namespace starmap