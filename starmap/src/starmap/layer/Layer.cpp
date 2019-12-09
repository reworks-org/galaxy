///
/// Layer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	void Layer::parse(const nlohmann::json& json)
	{
		

		if (json.count("draworder") > 0)
		{
			m_drawOrder = json.at("draworder");
		}

		m_height = json.at("height");
		m_id = json.at("id");


	} //end of parse()
}