///
/// RuntimeLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/PhysicsSystem.hpp"
#include "galaxy/systems/ScriptSystem.hpp"

#include "RuntimeLayer.hpp"

namespace galaxy
{
	namespace scene
	{
		RuntimeLayer::RuntimeLayer(std::string_view name, Scene* scene)
			: Layer {name, scene}
		{
			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_scene->m_camera);

			m_scene->m_world.create_system<systems::ScriptSystem>();
			m_scene->m_world.create_system<systems::AnimationSystem>();
			m_scene->m_world.create_system<systems::PhysicsSystem>();
			m_scene->m_world.create_system<systems::RenderSystem>();

			m_scene->m_world.m_runtime_layer = this;
		}

		RuntimeLayer::~RuntimeLayer()
		{
		}

		void RuntimeLayer::on_push()
		{
		}

		void RuntimeLayer::on_pop()
		{
		}

		void RuntimeLayer::events()
		{
			m_scene->m_camera.process_events();
			core::ServiceLocator<core::Window>::ref().trigger_queued_events(m_dispatcher);
		}

		void RuntimeLayer::update()
		{
			m_scene->m_world.update();
		}

		void RuntimeLayer::render()
		{
		}

		const std::string& RuntimeLayer::get_type() const
		{
			static std::string type = "Runtime";
			return type;
		}

		nlohmann::json RuntimeLayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["type"]        = get_type();
			json["name"]        = m_name;

			return json;
		}

		void RuntimeLayer::deserialize(const nlohmann::json& json)
		{
		}
	} // namespace scene
} // namespace galaxy