///
/// RigidBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>

#include "RigidBody.hpp"

namespace galaxy
{
	namespace components
	{
		RigidBody::RigidBody() noexcept
			: Serializable {}
			, m_fixed_rotation {false}
			, m_body {nullptr}
		{
		}

		RigidBody::RigidBody(const nlohmann::json& json) noexcept
			: Serializable {}
			, m_fixed_rotation {false}
			, m_body {nullptr}
		{
			deserialize(json);
		}

		RigidBody::RigidBody(RigidBody&& rb) noexcept
			: Serializable {}
			, m_fixed_rotation {false}
			, m_body {nullptr}
		{
			this->m_body           = rb.m_body;
			this->m_def            = std::move(rb.m_def);
			this->m_fixed_rotation = rb.m_fixed_rotation;

			rb.m_body = nullptr;
		}

		RigidBody& RigidBody::operator=(RigidBody&& rb) noexcept
		{
			if (this != &rb)
			{
				this->m_body           = rb.m_body;
				this->m_def            = std::move(rb.m_def);
				this->m_fixed_rotation = rb.m_fixed_rotation;

				rb.m_body = nullptr;
			}

			return *this;
		}

		RigidBody::~RigidBody() noexcept
		{
			// b2Body cleaned up by entt.
		}

		void RigidBody::set_material(const physics::Material& material) noexcept
		{
			m_material = material;

			update_fixture();
		}

		void RigidBody::set_shape(const float hw, const float hh) noexcept
		{
			m_shape.x = hw;
			m_shape.y = hh;

			update_fixture();
		}

		nlohmann::json RigidBody::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["fixed_rotation"] = m_fixed_rotation;
			json["shape"]["hw"]    = m_shape.x;
			json["shape"]["hh"]    = m_shape.y;

			if (m_body)
			{
				const auto& pos  = m_body->GetPosition();
				json["pos"]["x"] = pos.x;
				json["pos"]["y"] = pos.y;
				json["angle"]    = m_body->GetAngle();
			}

			json["material"] = m_material.serialize();
			json["type"]     = magic_enum::enum_name(m_def.type);

			return json;
		}

		void RigidBody::deserialize(const nlohmann::json& json)
		{
			const auto type = magic_enum::enum_cast<b2BodyType>(json.at("type").get<std::string>());
			if (type.has_value())
			{
				m_def.type = type.value();
			}

			const auto& pos = json.at("pos");
			m_def.position.Set(pos.at("x"), pos.at("y"));
			m_def.angle = json.at("angle");

			m_material.deserialize(json.at("material"));
			m_fixed_rotation = json.at("fixed_rotation");

			const auto& shape = json.at("shape");
			m_shape.x         = shape.at("hw");
			m_shape.y         = shape.at("hh");

			update_fixture();
		}

		void RigidBody::update_fixture() noexcept
		{
			if (m_body)
			{
				b2PolygonShape shape;
				shape.SetAsBox(m_shape.x, m_shape.y);

				b2FixtureDef fixture;
				fixture.shape                = &shape;
				fixture.density              = m_material.m_density;
				fixture.friction             = m_material.m_friction;
				fixture.restitution          = m_material.m_restitution;
				fixture.restitutionThreshold = m_material.m_restitution_threshold;

				m_body->CreateFixture(&fixture);
			}
		}
	} // namespace components
} // namespace galaxy