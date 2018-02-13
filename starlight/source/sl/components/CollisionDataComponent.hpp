///
/// CollisionDataComponent.hpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_COLLISIONDATACOMPONENT_HPP_
#define STARLIGHT_COLLISIONDATACOMPONENT_HPP_

#include <functional>

namespace entt { typedef std::uint32_t Entity }

namespace sl
{
	class CollisionDataComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param onCollision Lamba on what to do when two entitys collide.
		///
		CollisionDataComponent(std::function<void(entt::Entity, entt::Entity)> onCollision);

		///
		/// Destructor.
		///
		~CollisionDataComponent() = default;

		///
		/// Default move assignment overload.
		///
		CollisionDataComponent& operator=(const CollisionDataComponent&);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		CollisionDataComponent() = delete;

	public:
		std::function<void(entt::Entity, entt::Entity)> m_onCollision;
	};
}

#endif