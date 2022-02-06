///
/// Actions.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "Actions.hpp"

namespace galaxy
{
	namespace components
	{
		Actions::Actions() noexcept
			: Serializable {this}
		{
		}

		Actions::Actions(const nlohmann::json& json)
			: Serializable {this}
		{
			deserialize(json);
		}

		Actions::Actions(Actions&& a) noexcept
			: Serializable {this}
		{
			this->m_key_actions   = std::move(a.m_key_actions);
			this->m_mouse_actions = std::move(a.m_mouse_actions);
		}

		Actions& Actions::operator=(Actions&& a) noexcept
		{
			if (this != &a)
			{
				this->m_key_actions   = std::move(a.m_key_actions);
				this->m_mouse_actions = std::move(a.m_mouse_actions);
			}

			return *this;
		}

		void Actions::add_key_action(const input::Keys key, std::string_view func) noexcept
		{
			m_key_actions.emplace(key, func);
		}

		void Actions::add_mouse_action(const input::MouseButtons mouse_button, std::string_view func) noexcept
		{
			m_mouse_actions.emplace(mouse_button, func);
		}

		nlohmann::json Actions::serialize()
		{
			nlohmann::json json   = "{}"_json;
			json["key-actions"]   = nlohmann::json::object();
			json["mouse-actions"] = nlohmann::json::object();

			for (const auto& [key, action] : m_key_actions)
			{
				json["key-actions"][static_cast<std::string>(magic_enum::enum_name(key))] = action;
			}

			for (const auto& [mouse, action] : m_mouse_actions)
			{
				json["mouse-actions"][static_cast<std::string>(magic_enum::enum_name(mouse))] = action;
			}

			return json;
		}

		void Actions::deserialize(const nlohmann::json& json)
		{
			m_key_actions.clear();
			m_mouse_actions.clear();

			for (const auto& [key, action] : json.at("key-actions").items())
			{
				m_key_actions.emplace(magic_enum::enum_cast<input::Keys>(key).value(), action);
			}

			for (const auto& [mouse, action] : json.at("mouse-actions").items())
			{
				m_mouse_actions.emplace(magic_enum::enum_cast<input::MouseButtons>(mouse).value(), action);
			}
		}
	} // namespace components
} // namespace galaxy