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
		    : Serializable {this}, m_size {1.0f, 1.0f}, m_type {physics::BodyType::STATIC}
		{
		}

		RigidBody::RigidBody(const nlohmann::json& json)
		    : Serializable {this}, m_type {physics::BodyType::STATIC}
		{
			deserialize(json);
		}

		RigidBody::RigidBody(RigidBody&& rb) noexcept
		    : Serializable {this}
		{
			this->m_size = rb.m_size;
			this->m_aabb = std::move(rb.m_aabb);
			this->m_type = rb.m_type;
		}

		RigidBody& RigidBody::operator=(RigidBody&& rb) noexcept
		{
			if (this != &rb)
			{
				this->m_size = rb.m_size;
				this->m_aabb = std::move(rb.m_aabb);
				this->m_type = rb.m_type;
			}

			return *this;
		}

		const physics::AABB& RigidBody::update_aabb(const glm::mat4& transform)
		{
			const auto min = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			const auto max = transform * glm::vec4(m_size.x, m_size.y, 0.0f, 1.0f);

			m_aabb = {{min.x, min.y}, {max.x, max.y}};
			m_aabb.update_area();

			return m_aabb;
		}

		void RigidBody::set_bodytype(const physics::BodyType type) noexcept
		{
			m_type = type;
		}

		void RigidBody::set_size(const glm::vec2& size) noexcept
		{
			m_size = size;
		}

		const physics::AABB& RigidBody::get_aabb() const noexcept
		{
			return m_aabb;
		}

		const physics::BodyType RigidBody::get_type() const noexcept
		{
			return m_type;
		}

		nlohmann::json RigidBody::serialize()
		{
			// clang-format off
			nlohmann::json json = "{}"_json;
			json["type"] = magic_enum::enum_name(m_type);
			// clang-format on

			return json;
		}

		void RigidBody::deserialize(const nlohmann::json& json)
		{
			// clang-format off
			const std::string& type_str = json.at("type");
			m_type = magic_enum::enum_cast<physics::BodyType>(type_str).value();
			// clang-format on
		}
	} // namespace components
} // namespace galaxy