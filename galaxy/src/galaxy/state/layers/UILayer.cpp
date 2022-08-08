///
/// UILayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include "galaxy/core/Window.hpp"

#include "UILayer.hpp"

namespace galaxy
{
	namespace state
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
		}

		void UILayer::on_pop()
		{
		}

		void UILayer::events()
		{
			m_window->trigger_queued_events(m_dispatcher);
		}

		void UILayer::update()
		{
		}

		void UILayer::render()
		{
		}

		nlohmann::json UILayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = "UI"; // Used by Layers stack.

			return json;
		}

		void UILayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
		}
	} // namespace state
} // namespace galaxy