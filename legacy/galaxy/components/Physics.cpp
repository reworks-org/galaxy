///
/// BodyWrapper.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/systems/PhysicsSystem.hpp"

#include "BodyWrapper.hpp"

namespace galaxy
{
	namespace physics
	{
		BodyWrapper::BodyWrapper()
		    : m_body {nullptr}
		{
		}

		BodyWrapper::BodyWrapper(const nlohmann::json& json)
		    : m_body {nullptr}
		{
			auto* phys_sys = SL_HANDLE.world()->get_system<systems::PhysicsSystem>();

			if (json.at("fixed") == true)
			{
				m_body = phys_sys->create<StaticBody>(json.at("density").get<float>(), glm::vec2 {json.at("width").get<float>(), json.at("height").get<float>()});
			}
			else
			{
				m_body = phys_sys->create<KineticBody>(json.at("density").get<float>(), glm::vec2 {json.at("width").get<float>(), json.at("height").get<float>()});
			}

			m_body->set_pos(json.at("init-x"), json.at("init-y"));
			m_body->m_restitution      = json.at("restitution");
			m_body->m_static_friction  = json.at("static-friction");
			m_body->m_dynamic_friction = json.at("dynamic-friction");
		}

		BodyWrapper::~BodyWrapper()
		{
			m_body = nullptr;
		}
	} // namespace physics
} // namespace galaxy