///
/// UILayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <RmlUi/Core.h>

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/state/Scene.hpp"

#include "UILayer.hpp"

namespace galaxy
{
	namespace state
	{
		UILayer::UILayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
		{
			auto& window = core::ServiceLocator<core::Window>::ref();

			m_rml = Rml::CreateContext(std::format("{0}_RmlContext", m_name), {window.get_width(), window.get_height()});
			m_rml_events.set_context(m_rml);

			m_world.m_dispatcher.sink<events::MouseMoved>().connect<&ui::RMLEvents::on_mouse_move>(m_rml_events);
			m_world.m_dispatcher.sink<events::MousePressed>().connect<&ui::RMLEvents::on_mouse_pressed>(m_rml_events);
			m_world.m_dispatcher.sink<events::MouseReleased>().connect<&ui::RMLEvents::on_mouse_released>(m_rml_events);
			m_world.m_dispatcher.sink<events::MouseWheel>().connect<&ui::RMLEvents::on_mouse_wheel>(m_rml_events);
			m_world.m_dispatcher.sink<events::KeyDown>().connect<&ui::RMLEvents::on_key_down>(m_rml_events);
			m_world.m_dispatcher.sink<events::KeyUp>().connect<&ui::RMLEvents::on_key_up>(m_rml_events);
			m_world.m_dispatcher.sink<events::KeyChar>().connect<&ui::RMLEvents::on_key_char>(m_rml_events);
			m_world.m_dispatcher.sink<events::WindowResized>().connect<&ui::RMLEvents::on_window_resize>(m_rml_events);

			m_world.m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(scene->get_camera());
			m_world.m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(scene->get_camera());
		}

		UILayer::~UILayer() noexcept
		{
			m_rml = nullptr;
		}

		void UILayer::on_push()
		{
			// show rml docs
		}

		void UILayer::on_pop()
		{
			// hide rml docs
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();
			ae.stop_all();
		}

		void UILayer::events()
		{
			core::ServiceLocator<core::Window>::ref().trigger_queued_events(m_world.m_dispatcher);
		}

		void UILayer::update()
		{
			m_world.update_systems(this);
			m_rml->Update();
		}

		void UILayer::render()
		{
			// draw rml docs?
			m_rml->Render();
		}

		nlohmann::json UILayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = "UI"; // Used by Layers stack.
			json["world"]       = m_world.serialize();

			// serialize rml docs?

			return json;
		}

		void UILayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_world.deserialize(json.at("world"));
		}
	} // namespace state
} // namespace galaxy