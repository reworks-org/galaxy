///
/// Physics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/scripting/JSONUtils.hpp"

#include "Physics.hpp"

namespace galaxy
{
	namespace components
	{
		Physics::Physics() noexcept
		    : Serializable {this}, m_world_pointer {nullptr}, m_body {nullptr}
		{
		}

		Physics::Physics(Physics&& p) noexcept
		    : Serializable {this}
		{
			this->m_world_pointer = p.m_world_pointer;
			this->m_body          = p.m_body;

			p.m_body = nullptr;
		}

		Physics& Physics::operator=(Physics&& p) noexcept
		{
			if (this != &p)
			{
				this->m_world_pointer = p.m_world_pointer;
				this->m_body          = p.m_body;

				p.m_body = nullptr;
			}

			return *this;
		}

		Physics::~Physics()
		{
			if (m_body != nullptr)
			{
				m_world_pointer->DestroyBody(m_body);
				m_body = nullptr;
			}
		}

		void Physics::create(const physics::BodyConfig& config)
		{
			if (!m_world_pointer)
			{
				GALAXY_LOG(GALAXY_FATAL, "This component was not created through core::World.");
			}
			else
			{
				m_body = m_world_pointer->CreateBody(&config.m_def);
				m_body->CreateFixture(&config.m_fixture_def);
			}
		}

		void Physics::create_from_json(std::string_view file)
		{
			const auto json = json::parse_from_disk(file);
			if (json == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load and parse json file for physics component: {0}.", file);
			}
			else
			{
				parse_json(json.value());
			}
		}

		b2Body* const Physics::body()
		{
			if (!m_body)
			{
				GALAXY_LOG(GALAXY_FATAL, "Did not create physics component before calling body().");
			}

			return m_body;
		}

		nlohmann::json Physics::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["enabled"]        = m_body->IsEnabled();
			json["x"]              = m_body->GetPosition().x * physics::FROM_METERS_TO_PIXELS;
			json["y"]              = m_body->GetPosition().y * physics::FROM_METERS_TO_PIXELS;
			json["angle"]          = glm::degrees(m_body->GetAngle());
			json["fixed-rotation"] = m_body->IsFixedRotation();

			const auto body_type = m_body->GetType();
			if (body_type == b2_staticBody)
			{
				json["body"] = "static";
			}
			else if (body_type == b2_dynamicBody)
			{
				json["body"] = "dynamic";
			}
			else if (body_type == b2_kinematicBody)
			{
				json["body"] = "kinematic";
			}

			json["shape"] = m_bodyshape;
			if (m_bodyshape == "polygon")
			{
				json["vertexs"] = nlohmann::json::array();
				for (const auto& vec2 : m_vertexs)
				{
					const float x = vec2.x * physics::FROM_METERS_TO_PIXELS;
					const float y = vec2.y * physics::FROM_METERS_TO_PIXELS;

					json["vertexs"].push_back(nlohmann::json::array({x, y}));
				}
			}
			else if (m_bodyshape == "rect")
			{
				json["half-width"]  = m_hwhh.x * physics::FROM_METERS_TO_PIXELS;
				json["half-height"] = m_hwhh.y * physics::FROM_METERS_TO_PIXELS;
			}
			else if (m_bodyshape == "circle")
			{
				auto* as_circle = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
				json["radius"]  = as_circle->m_radius * physics::FROM_METERS_TO_PIXELS;
			}

			json["density"]  = m_body->GetFixtureList()->GetDensity();
			json["friction"] = m_body->GetFixtureList()->GetFriction();
			json["bounce"]   = m_body->GetFixtureList()->GetRestitution();

			return json;
		}

		void Physics::deserialize(const nlohmann::json& json)
		{
			parse_json(json);
		}

		void Physics::parse_json(const nlohmann::json& json)
		{
			if (m_body != nullptr)
			{
				m_world_pointer->DestroyBody(m_body);
				m_body = nullptr;
			}

			physics::BodyConfig config;
			config.m_def.enabled = json.at("enabled");

			const float x = json.at("x").get<float>() * physics::FROM_PIXELS_TO_METERS;
			const float y = json.at("y").get<float>() * physics::FROM_PIXELS_TO_METERS;
			config.m_def.position.Set(x, y);
			config.m_def.angle         = glm::radians(json.at("angle").get<float>());
			config.m_def.fixedRotation = json.at("fixed-rotation");

			const std::string body_type = json.at("body");
			if (body_type == "static")
			{
				config.m_def.type = b2_staticBody;
			}
			else if (body_type == "dynamic")
			{
				config.m_def.type = b2_dynamicBody;
			}
			else if (body_type == "kinematic")
			{
				config.m_def.type = b2_kinematicBody;
			}

			m_bodyshape = json.at("shape");
			if (m_bodyshape == "polygon")
			{
				config.m_shape   = std::make_shared<b2PolygonShape>();
				auto* as_polygon = static_cast<b2PolygonShape*>(config.m_shape.get());

				m_vertexs.clear();
				for (const auto& arr : json.at("vertexs"))
				{
					const float x = arr[0].get<float>() * physics::FROM_PIXELS_TO_METERS;
					const float y = arr[1].get<float>() * physics::FROM_PIXELS_TO_METERS;
					m_vertexs.emplace_back(x, y);
				}

				as_polygon->Set(m_vertexs.data(), m_vertexs.size());
			}
			else if (m_bodyshape == "rect")
			{
				config.m_shape = std::make_shared<b2PolygonShape>();
				auto* as_rect  = static_cast<b2PolygonShape*>(config.m_shape.get());

				m_hwhh.x = json.at("half-width").get<float>() * physics::FROM_PIXELS_TO_METERS;
				m_hwhh.y = json.at("half-height").get<float>() * physics::FROM_PIXELS_TO_METERS;

				as_rect->SetAsBox(m_hwhh.x, m_hwhh.y);
			}
			else if (m_bodyshape == "circle")
			{
				config.m_shape  = std::make_shared<b2CircleShape>();
				auto* as_circle = static_cast<b2CircleShape*>(config.m_shape.get());

				const float radius  = json.at("radius").get<float>() * physics::FROM_PIXELS_TO_METERS;
				as_circle->m_radius = radius;
			}

			config.m_fixture_def.shape       = config.m_shape.get();
			config.m_fixture_def.density     = json.at("density");
			config.m_fixture_def.friction    = std::clamp(json.at("friction").get<float>(), 0.0f, 1.0f);
			config.m_fixture_def.restitution = std::clamp(json.at("bounce").get<float>(), 0.0f, 1.0f);

			create(config);
		}
	} // namespace components
} // namespace galaxy