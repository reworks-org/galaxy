///
/// UILayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <RmlUi/Core.h>

#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"

#include "UILayer.hpp"

namespace galaxy
{
	namespace state
	{
		UILayer::UILayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
		{
			m_rml = Rml::CreateContext(std::format("{0}_RmlContext", m_name), {m_window->get_width(), m_window->get_height()});
			m_rml_events.set_context(m_rml);

			m_dispatcher.sink<events::MouseMoved>().connect<&ui::RMLEvents::on_mouse_move>(m_rml_events);
			m_dispatcher.sink<events::MousePressed>().connect<&ui::RMLEvents::on_mouse_pressed>(m_rml_events);
			m_dispatcher.sink<events::MouseReleased>().connect<&ui::RMLEvents::on_mouse_released>(m_rml_events);
			m_dispatcher.sink<events::MouseWheel>().connect<&ui::RMLEvents::on_mouse_wheel>(m_rml_events);
			m_dispatcher.sink<events::KeyDown>().connect<&ui::RMLEvents::on_key_down>(m_rml_events);
			m_dispatcher.sink<events::KeyUp>().connect<&ui::RMLEvents::on_key_up>(m_rml_events);
			m_dispatcher.sink<events::KeyChar>().connect<&ui::RMLEvents::on_key_char>(m_rml_events);
			m_dispatcher.sink<events::WindowResized>().connect<&ui::RMLEvents::on_window_resize>(m_rml_events);
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
		}

		void UILayer::events()
		{
			m_window->trigger_queued_events(m_dispatcher);
		}

		void UILayer::update()
		{
			m_world.update_systems(m_scene);
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