///
/// CollisionDataComponent.cpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "CollisionDataComponent.hpp"

namespace sl
{
	CollisionDataComponent::CollisionDataComponent(std::function<void(entt::Entity, entt::Entity)> onCollision)
	{
		m_onCollision = onCollision;
	}

	CollisionDataComponent& CollisionDataComponent::operator=(const CollisionDataComponent&)
	{
		return *this;
	}
}