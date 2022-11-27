///
/// RigidBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>
#include <nlohmann/json.hpp>

#include "RigidBody.hpp"

namespace galaxy
{
	namespace components
	{
		RigidBody::RigidBody() noexcept
			: Serializable {}
			, m_shape {0, 0}
			, m_type {b2_staticBody}
			, m_fixed_rotation {true}
			, m_density {1.0f}
			, m_friction {0.0f}
			, m_restitution {0.0f}
			, m_restitution_threshold {0.0f}
			, m_bullet {false}
			, m_body {nullptr}
		{
		}

		RigidBody::RigidBody(const nlohmann::json& json) noexcept
			: Serializable {}
		{
			deserialize(json);
		}

		RigidBody::RigidBody(RigidBody&& rb) noexcept
			: Serializable {}
		{
			this->m_shape                 = std::move(rb.m_shape);
			this->m_type                  = rb.m_type;
			this->m_fixed_rotation        = rb.m_fixed_rotation;
			this->m_density               = rb.m_density;
			this->m_friction              = rb.m_friction;
			this->m_restitution           = rb.m_restitution;
			this->m_restitution_threshold = rb.m_restitution_threshold;
			this->m_bullet                = rb.m_bullet;
			this->m_body                  = rb.m_body;

			rb.m_body = nullptr;
		}

		RigidBody& RigidBody::operator=(RigidBody&& rb) noexcept
		{
			if (this != &rb)
			{
				this->m_shape                 = std::move(rb.m_shape);
				this->m_type                  = rb.m_type;
				this->m_fixed_rotation        = rb.m_fixed_rotation;
				this->m_density               = rb.m_density;
				this->m_friction              = rb.m_friction;
				this->m_restitution           = rb.m_restitution;
				this->m_restitution_threshold = rb.m_restitution_threshold;
				this->m_bullet                = rb.m_bullet;
				this->m_body                  = rb.m_body;

				rb.m_body = nullptr;
			}

			return *this;
		}

		RigidBody::~RigidBody() noexcept
		{
			// b2Body cleaned up by entt.
		}

		nlohmann::json RigidBody::serialize()
		{
			nlohmann::json json           = "{}"_json;
			json["shape"]["hw"]           = m_shape.x;
			json["shape"]["hh"]           = m_shape.y;
			json["type"]                  = magic_enum::enum_name(m_type);
			json["fixed_rotation"]        = m_fixed_rotation;
			json["density"]               = m_density;
			json["friction"]              = m_friction;
			json["restitution"]           = m_restitution;
			json["restitution_threshold"] = m_restitution_threshold;
			json["bullet"]                = m_bullet;

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

			m_fixed_rotation        = json.at("fixed_rotation");
			m_density               = json.at("density");
			m_friction              = json.at("friction");
			m_restitution           = json.at("restitution");
			m_restitution_threshold = json.at("restitution_threshold");
			m_bullet                = json.at("bullet");
		}
	} // namespace components
} // namespace galaxy