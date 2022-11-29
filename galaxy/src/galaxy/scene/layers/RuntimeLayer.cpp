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
#include "galaxy/systems/ScriptSystem.hpp"

#include "RuntimeLayer.hpp"

namespace galaxy
{
	namespace scene
	{
		RuntimeLayer::RuntimeLayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
			, m_map {this}
		{
			m_world.create_system<systems::ScriptSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::RenderSystem>();
		}

		RuntimeLayer::~RuntimeLayer() noexcept
		{
		}

		void RuntimeLayer::on_push()
		{
		}

		void RuntimeLayer::on_pop()
		{
			core::ServiceLocator<audio::AudioEngine>::ref().stop();
		}

		void RuntimeLayer::events()
		{
			core::ServiceLocator<core::Window>::ref().trigger_queued_events(m_world.m_dispatcher);
		}

		void RuntimeLayer::update()
		{
			m_world.update(this);
		}

		void RuntimeLayer::render()
		{
		}

		const std::string& RuntimeLayer::get_type() const noexcept
		{
			static std::string type = "Runtime";
			return type;
		}

		nlohmann::json RuntimeLayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = get_type();
			json["world"]       = m_world.serialize();
			json["active_map"]  = m_map.get_name();

			return json;
		}

		void RuntimeLayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_world.deserialize(json.at("world"));
			m_map.load_map(json.at("active_map"));
		}
	} // namespace scene
} // namespace galaxy