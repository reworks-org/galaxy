///
/// PhysicsComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	PhysicsComponent::PhysicsComponent()
	    : m_body {nullptr}, m_world {nullptr}
	{
		m_world = &SL_HANDLE.world()->phys_world();
	}

	PhysicsComponent::PhysicsComponent(const nlohmann::json& json)
	    : m_body {nullptr}, m_world {nullptr}
	{
		m_world = &SL_HANDLE.world()->phys_world();

		if (json.at("fixed") == true)
		{
			m_body = m_world->create<rs::StaticBody>(json.at("density").get<float>(), glm::vec2 {json.at("width").get<float>(), json.at("height").get<float>()});
		}
		else
		{
			m_body = m_world->create<rs::KineticBody>(json.at("density").get<float>(), glm::vec2 {json.at("width").get<float>(), json.at("height").get<float>()});
		}

		m_body->set_pos(json.at("init-x"), json.at("init-y"));
		m_body->m_restitution      = json.at("restitution");
		m_body->m_static_friction  = json.at("static-friction");
		m_body->m_dynamic_friction = json.at("dynamic-friction");
	}

	PhysicsComponent::PhysicsComponent(PhysicsComponent&& pc)
	{
		this->m_body  = std::move(pc.m_body);
		this->m_world = pc.m_world;

		pc.m_body  = nullptr;
		pc.m_world = nullptr;
	}

	PhysicsComponent& PhysicsComponent::operator=(PhysicsComponent&& pc)
	{
		if (this != &pc)
		{
			this->m_body  = std::move(pc.m_body);
			this->m_world = pc.m_world;

			pc.m_body  = nullptr;
			pc.m_world = nullptr;
		}

		return *this;
	}

	PhysicsComponent::~PhysicsComponent()
	{
		m_body  = nullptr;
		m_world = nullptr;
	}
} // namespace galaxy