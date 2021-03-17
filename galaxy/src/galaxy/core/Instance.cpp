///
/// Instance.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Instance.hpp"

namespace galaxy
{
	namespace core
	{
		Instance::~Instance()
		{
			m_scene_stack.clear();
		}

		void Instance::set_name(std::string_view name) noexcept
		{
			m_name = static_cast<std::string>(name);
		}

		const std::string& Instance::get_name() const noexcept
		{
			return m_name;
		}

		core::SceneStack& Instance::get_stack() noexcept
		{
			return m_scene_stack;
		}

		nlohmann::json Instance::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_scene_stack.serialize();

			return json;
		}

		void Instance::deserialize(const nlohmann::json& json)
		{
			m_scene_stack.clear();

			m_name = json.at("name");
			m_scene_stack.deserialize(json.at("stack"));
		}

		Instance::Instance() noexcept
		    : Serializable {this}, m_name {"default"}
		{
		}
	} // namespace core
} // namespace galaxy