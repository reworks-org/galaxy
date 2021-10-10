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
			: Serializable {this}
			, m_type {physics::BodyType::STATIC}
		{
		}

		RigidBody::RigidBody(const nlohmann::json& json)
			: Serializable {this}
			, m_type {physics::BodyType::STATIC}
		{
			deserialize(json);
		}

		RigidBody::RigidBody(RigidBody&& rb) noexcept
			: Serializable {this}
		{
			this->m_type = rb.m_type;
		}

		RigidBody& RigidBody::operator=(RigidBody&& rb) noexcept
		{
			if (this != &rb)
			{
				this->m_type = rb.m_type;
			}

			return *this;
		}

		nlohmann::json RigidBody::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["type"]        = magic_enum::enum_name(m_type);

			return json;
		}

		void RigidBody::deserialize(const nlohmann::json& json)
		{
			m_type = magic_enum::enum_cast<physics::BodyType>(json.at("type").get<std::string>()).value();
		}
	} // namespace components
} // namespace galaxy