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

#include "RuntimeLayer.hpp"

namespace galaxy
{
	namespace state
	{
		RuntimeLayer::RuntimeLayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
		{
		}

		RuntimeLayer::~RuntimeLayer() noexcept
		{
		}

		void RuntimeLayer::on_push()
		{
		}

		void RuntimeLayer::on_pop()
		{
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();
			ae.stop_all();
		}

		void RuntimeLayer::events()
		{
			m_window->trigger_queued_events(m_world.m_dispatcher);
		}

		void RuntimeLayer::update()
		{
			m_world.update_systems(this);
		}

		void RuntimeLayer::render()
		{
		}

		nlohmann::json RuntimeLayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = "Runtime"; // Used by Layers stack.
			json["world"]       = m_world.serialize();

			return json;
		}

		void RuntimeLayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_world.deserialize(json.at("world"));
		}
	} // namespace state
} // namespace galaxy