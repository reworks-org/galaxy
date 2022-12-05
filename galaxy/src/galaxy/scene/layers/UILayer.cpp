///
/// UILayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/scene/Scene.hpp"

#include "UILayer.hpp"

namespace galaxy
{
	namespace scene
	{
		UILayer::UILayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
		{
		}

		UILayer::~UILayer() noexcept
		{
		}

		void UILayer::on_push()
		{
			// show rml docs
		}

		void UILayer::on_pop()
		{
			// hide rml docs
			core::ServiceLocator<audio::AudioEngine>::ref().stop();
		}

		void UILayer::events()
		{
			core::ServiceLocator<core::Window>::ref().trigger_queued_events(m_world.m_dispatcher);
		}

		void UILayer::update()
		{
			m_world.update(this);
		}

		void UILayer::render()
		{
		}

		const std::string& UILayer::get_type() const noexcept
		{
			static std::string type = "UI";
			return type;
		}

		nlohmann::json UILayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = get_type();
			json["world"]       = m_world.serialize();

			// serialize rml docs?

			return json;
		}

		void UILayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_world.deserialize(json.at("world"));
		}
	} // namespace scene
} // namespace galaxy