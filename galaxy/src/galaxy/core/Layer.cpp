///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layer.hpp"

namespace galaxy
{
	namespace core
	{
		Layer::Layer(Application* app) noexcept
			: Serializable {this}
			, m_app {app}
			, m_name {"New Layer"}
		{
		}

		Layer::~Layer()
		{
			m_scene_stack.clear();
		}

		void Layer::set_name(std::string_view name) noexcept
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Layer::get_name() const noexcept
		{
			return m_name;
		}

		SceneStack& Layer::get_stack() noexcept
		{
			return m_scene_stack;
		}

		nlohmann::json Layer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_scene_stack.serialize();

			return json;
		}

		void Layer::deserialize(const nlohmann::json& json)
		{
			m_scene_stack.clear();

			m_name = json.at("name");
			m_scene_stack.deserialize(json.at("stack"));
		}
	} // namespace core
} // namespace galaxy