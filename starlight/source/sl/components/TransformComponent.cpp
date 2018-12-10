///
/// TransformComponent.cpp
/// starlight
///
/// Created by reworks on 30/09/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"

#include "TransformComponent.hpp"

namespace sl
{
	TransformComponent::TransformComponent()
		:m_layer(0), m_rect(0.0f, 0.0f, 0, 0), m_angle(0.0f)
	{
	}

	TransformComponent::TransformComponent(const sol::table& table)
		: m_layer(0), m_rect(0.0f, 0.0f, 0, 0), m_angle(0.0f)
	{
		// Retrieve the values from the lua table.
		m_layer = table.get<int>("layer");
		m_rect.m_x = table.get<float>("x");
		m_rect.m_y = table.get<float>("y");
		m_rect.m_width = table.get<int>("width");
		m_rect.m_height = table.get<int>("height");
		m_angle = table.get<float>("angle");
	}

	TransformComponent::TransformComponent(int layer, float angle, const Rect<float, int>& rect)
		:m_layer(layer), m_angle(angle), m_rect(rect)
	{
	}
}