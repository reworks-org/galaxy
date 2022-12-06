///
/// UILayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

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
		}

		void UILayer::on_pop()
		{
		}

		void UILayer::events()
		{
		}

		void UILayer::update()
		{
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
			json["type"]        = get_type();
			json["name"]        = m_name;

			return json;
		}

		void UILayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
		}
	} // namespace scene
} // namespace galaxy