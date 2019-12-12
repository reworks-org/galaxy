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
	const int Layer::getHeight() const noexcept
	{
		return m_height;
	}

	const int Layer::getID() const noexcept
	{
		return m_id;
	}

	const std::string Layer::getName() const noexcept
	{
		return m_name;
	}

	Layer::Layer() noexcept
		:m_height(0), m_id(0), m_name("")
	{
	}

	Layer::Layer(const nlohmann::json& json)
		:m_height(0), m_id(0), m_name("")
	{
		m_height = json.at("height");
		m_id = json.at("id");
		m_name = json.at("name");
	}
}