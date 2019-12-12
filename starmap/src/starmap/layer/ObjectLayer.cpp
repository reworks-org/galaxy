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
	ObjectLayer::ObjectLayer(const nlohmann::json& json)
		:Layer(json), m_drawOrder("")
	{
		if (json.count("draworder") > 0)
		{
			m_drawOrder = json.at("draworder");
		}

		// load objects...
	}
	
	ObjectLayer::~ObjectLayer() noexcept
	{
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