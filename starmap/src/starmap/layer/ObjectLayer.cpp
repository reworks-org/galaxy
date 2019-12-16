///
/// ObjectLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "ObjectLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	ObjectLayer::ObjectLayer()
	{
		throw std::runtime_error("Cannot instantiate a default constructed ObjectLayer!");
	}

	ObjectLayer::ObjectLayer(const nlohmann::json& json)
		:Layer(json), m_drawOrder("")
	{
		if (json.count("draworder") > 0)
		{
			m_drawOrder = json.at("draworder");
		}

		if (json.count("objects") > 0)
		{
			auto objectArray = json.at("objects");
			std::for_each(objectArray.begin(), objectArray.end(), [&](const nlohmann::json& object)
			{
				m_objects.emplace_back(object);
			});
		}
	}
	
	ObjectLayer::~ObjectLayer() noexcept
	{
		m_objects.clear();
	}

	const std::string& ObjectLayer::getCompression() const noexcept
	{
		return m_drawOrder;
	}

	const auto& ObjectLayer::getObjects() const noexcept
	{
		return m_objects;
	}
}