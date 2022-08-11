///
/// SandboxLayer.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include <galaxy/core/Window.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/events/KeyDown.hpp>

#include "SandboxLayer.hpp"

using namespace galaxy;

namespace sandbox
{
	void close(events::KeyDown& kd)
	{
		if (kd.m_keycode == input::Keys::ESCAPE)
		{
			auto& window = core::ServiceLocator<core::Window>::ref();
			window.close();

			kd.m_handled = true;
		}
	}

	SandboxLayer::SandboxLayer(std::string_view name, state::Scene* scene) noexcept
		: state::Layer {name, scene}
	{
		m_dispatcher.sink<events::KeyDown>().connect<&close>();
	}

	SandboxLayer::~SandboxLayer() noexcept
	{
	}

	void SandboxLayer::on_push()
	{
	}

	void SandboxLayer::on_pop()
	{
	}

	void SandboxLayer::events()
	{
		m_window->trigger_queued_events(m_dispatcher);
	}

	void SandboxLayer::update()
	{
	}

	void SandboxLayer::render()
	{
	}

	nlohmann::json SandboxLayer::serialize()
	{
		nlohmann::json json = "{}"_json;
		json["name"]        = m_name;
		json["type"]        = "Sandbox";

		return json;
	}

	void SandboxLayer::deserialize(const nlohmann::json& json)
	{
		m_name = json.at("name");
	}
} // namespace sandbox