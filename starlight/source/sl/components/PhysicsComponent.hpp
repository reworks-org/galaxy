///
/// PhysicsComponent.hpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PHYSICSCOMPONENT_HPP_
#define STARLIGHT_PHYSICSCOMPONENT_HPP_

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

class b2Body;
namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class PhysicsComponent final
	{
		friend class cereal::access;

	public:
		///
		/// Constructor.
		///
		/// \param entity Entity to attach to component fixtures.
		/// \param table sol::table containing data.
		///
		PhysicsComponent(entt::Entity entity, const sol::table& table);

		///
		/// \brief Destructor.
		///
		/// CLEANUP IS HANDLED BY BOX2D MANAGER. THIS IS BECAUSE WE DONT KNOW WHEN DESTRUCTORS ARE CALLED, AND CLEANUP CODE MUST BE CALLED BEFORE 
		/// CLEANUP OF BOX2DMANAGER!
		///
		~PhysicsComponent() = default;

		///
		/// Default move assignment overload.
		///
		PhysicsComponent& operator=(const PhysicsComponent&);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		PhysicsComponent() = delete;

		///
		/// This method lets cereal know which data members to save.
		///
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(m_body->GetPosition().x, m_body->GetPosition().y, m_body->GetAngle(), m_body->GetAngularVelocity(), m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
		}

		///
		/// This method lets cereal know which data members to load.
		///
		template<class Archive>
		void load(Archive& archive)
		{
			float32 x, y, angle, angleVel, linearX, linearY = 0.0f;

			archive(x, y, angle, angleVel, linearX, linearY);

			m_body->SetTransform(b2Vec2(x, y), angle);
			m_body->SetAngularVelocity(angleVel);
			m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
		}

	public:
		b2Body* m_body;
	};
}

#endif