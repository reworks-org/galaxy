///
/// RigidBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "RigidBody.hpp"

namespace galaxy
{
	namespace components
	{
		RigidBody::RigidBody()
			: Serializable {}
			, m_shape {0, 0}
			, m_type {b2_staticBody}
			, m_density {1.0f}
			, m_friction {0.0f}
			, m_restitution {0.0f}
			, m_restitution_threshold {0.0f}
			, m_bullet {false}
			, m_fixed_rotation {true}
			, m_body {nullptr}
		{
		}

		RigidBody::RigidBody(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		RigidBody::RigidBody(RigidBody&& rb)
			: Serializable {}
		{
			this->m_shape                 = std::move(rb.m_shape);
			this->m_type                  = rb.m_type;
			this->m_density               = rb.m_density;
			this->m_friction              = rb.m_friction;
			this->m_restitution           = rb.m_restitution;
			this->m_restitution_threshold = rb.m_restitution_threshold;
			this->m_bullet                = rb.m_bullet;
			this->m_fixed_rotation        = rb.m_fixed_rotation;
			this->m_body                  = rb.m_body;

			rb.m_body = nullptr;
		}

		RigidBody& RigidBody::operator=(RigidBody&& rb)
		{
			if (this != &rb)
			{
				this->m_shape                 = std::move(rb.m_shape);
				this->m_type                  = rb.m_type;
				this->m_density               = rb.m_density;
				this->m_friction              = rb.m_friction;
				this->m_restitution           = rb.m_restitution;
				this->m_restitution_threshold = rb.m_restitution_threshold;
				this->m_bullet                = rb.m_bullet;
				this->m_fixed_rotation        = rb.m_fixed_rotation;
				this->m_body                  = rb.m_body;

				rb.m_body = nullptr;
			}

			return *this;
		}

		RigidBody::~RigidBody()
		{
			// b2Body cleaned up by entt.
		}

		void RigidBody::set_shape(const float hw, const float hh)
		{
			m_shape.x = hw;
			m_shape.y = hh;

			auto fixture = m_body->GetFixtureList();
			auto shape   = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
			shape->SetAsBox(m_shape.x, m_shape.y);
		}

		void RigidBody::set_type(const b2BodyType type)
		{
			m_body->SetType(type);
		}

		void RigidBody::set_density(const float density)
		{
			m_density = density;

			auto fixture = m_body->GetFixtureList();
			fixture->SetDensity(m_density);

			m_body->ResetMassData();
		}

		void RigidBody::set_friction(const float friction)
		{
			m_friction = friction;

			auto fixture = m_body->GetFixtureList();
			fixture->SetFriction(m_friction);
		}

		void RigidBody::set_restitution(const float restitution)
		{
			m_restitution = restitution;

			auto fixture = m_body->GetFixtureList();
			fixture->SetRestitution(m_restitution);
		}

		void RigidBody::set_restitution_threshold(const float restitution_threshold)
		{
			m_restitution_threshold = restitution_threshold;

			auto fixture = m_body->GetFixtureList();
			fixture->SetRestitutionThreshold(m_restitution_threshold);
		}

		void RigidBody::set_bullet(const bool is_bullet)
		{
			m_body->SetBullet(is_bullet);
		}

		void RigidBody::set_fixed_rotation(const bool fixed_rotation)
		{
			m_body->SetFixedRotation(fixed_rotation);
		}

		const glm::vec2& RigidBody::get_shape() const
		{
			return m_shape;
		}

		b2BodyType RigidBody::get_type() const
		{
			return m_type;
		}

		float RigidBody::get_density() const
		{
			return m_density;
		}

		float RigidBody::get_friction() const
		{
			return m_friction;
		}

		float RigidBody::get_restitution() const
		{
			return m_restitution;
		}

		float RigidBody::get_restitution_threshold() const
		{
			return m_restitution_threshold;
		}

		bool RigidBody::is_bullet() const
		{
			return m_bullet;
		}

		bool RigidBody::is_rotation_fixed() const
		{
			return m_fixed_rotation;
		}

		nlohmann::json RigidBody::serialize()
		{
			nlohmann::json json           = "{}"_json;
			json["shape"]["hw"]           = m_shape.x;
			json["shape"]["hh"]           = m_shape.y;
			json["type"]                  = magic_enum::enum_name(m_type);
			json["density"]               = m_density;
			json["friction"]              = m_friction;
			json["restitution"]           = m_restitution;
			json["restitution_threshold"] = m_restitution_threshold;
			json["bullet"]                = m_bullet;
			json["fixed_rotation"]        = m_fixed_rotation;

			return json;
		}

		void RigidBody::deserialize(const nlohmann::json& json)
		{
			const auto& shape = json.at("shape");
			m_shape.x         = shape.at("hw");
			m_shape.y         = shape.at("hh");

			const auto type = magic_enum::enum_cast<b2BodyType>(json.at("type").get<std::string>());
			if (type.has_value())
			{
				m_type = type.value();
			}

			m_density               = json.at("density");
			m_friction              = json.at("friction");
			m_restitution           = json.at("restitution");
			m_restitution_threshold = json.at("restitution_threshold");
			m_bullet                = json.at("bullet");
			m_fixed_rotation        = json.at("fixed_rotation");
		}
	} // namespace components
} // namespace galaxy
