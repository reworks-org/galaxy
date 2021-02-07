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
		    : m_world_pointer {nullptr}, m_body {nullptr}
		{
		}

		Physics::Physics(Physics&& p) noexcept
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
			auto json = json::parse_from_disk(file);
			parse_json(json);
		}

		b2Body* const Physics::body()
		{
			if (!m_body)
			{
				GALAXY_LOG(GALAXY_FATAL, "Did not create physics component before calling body().");
			}

			return m_body;
		}

		void Physics::parse_json(const nlohmann::json& json)
		{
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

			const std::string body_shape = json.at("shape");
			if (body_shape == "polygon")
			{
				config.m_shape   = std::make_shared<b2PolygonShape>();
				auto* as_polygon = static_cast<b2PolygonShape*>(config.m_shape.get());

				std::vector<b2Vec2> vertexs;
				for (const auto& arr : json.at("vertexs"))
				{
					const float x = arr[0].get<float>() * physics::FROM_PIXELS_TO_METERS;
					const float y = arr[1].get<float>() * physics::FROM_PIXELS_TO_METERS;
					vertexs.emplace_back(x, y);
				}

				as_polygon->Set(vertexs.data(), vertexs.size());
			}
			else if (body_shape == "rect")
			{
				config.m_shape = std::make_shared<b2PolygonShape>();
				auto* as_rect  = static_cast<b2PolygonShape*>(config.m_shape.get());

				const float hw = json.at("half-width").get<float>() * physics::FROM_PIXELS_TO_METERS;
				const float hh = json.at("half-height").get<float>() * physics::FROM_PIXELS_TO_METERS;

				as_rect->SetAsBox(hw, hh);
			}
			else if (body_shape == "circle")
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